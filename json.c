#include <stdio.h>   // 파일 처리 함수가 선언된 헤더 파일
#include <stdlib.h>  // malloc, free 함수가 선언된 헤더 파일
#include <stdbool.h> // bool, true, false가 정의된 헤더 파일
#include <string.h>  // strchr, memset, memcpy 함수가 선언된 헤더 파일

int totaltokensize = 0;
// 토큰 종류 열거형
typedef enum _TOKEN_TYPE
{
    TOKEN_STRING,
    TOKEN_PRIMITIVE,
    /*
    UNDEFINED = 0, 
    OBJECT = 1, 
    ARRAY = 2, 
    STRING = 3, 
    PRIMITIVE = 4
    */
} TOKEN_TYPE;

// 토큰 구조체
typedef struct _TOKEN
{
    TOKEN_TYPE type;
    // struct {
    //     char *string;
    //     float floatnum;
    //     int intnum;
    // };
    bool isArray;
    bool isArraytitle;
    int arraysize;
    int size;
    int start;
    int end;
} TOKEN;

#define TOKEN_COUNT 100 // 토큰의 최대 개수

// JSON 구조체
typedef struct _JSON
{
    TOKEN tokens[TOKEN_COUNT]; // 토큰 배열
} JSON;

char *readFile(char *filename, int *readSize) // 파일을 읽어서 내용을 반환하는 함수
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return NULL;

    int size;
    char *buffer;

    // get file size
    fseek(fp, 0, SEEK_END); // move file pointer from 0 to end fo file
    size = ftell(fp);       // get size of file
    fseek(fp, 0, SEEK_SET); // move file pointer back to 0

    buffer = malloc(size + 1);   // allcate file size + 1 buffer
    memset(buffer, 0, size + 1); // init buffer with 0

    if (fread(buffer, size, 1, fp) < 1)
    {
        // if fread fails, set readsize as 0, free buffer, and close file
        *readSize = 0;
        free(buffer);
        fclose(fp);
        return NULL;
    }

    // if fread success, give size to readSize of main function
    *readSize = size;

    fclose(fp); // close file

    return buffer; // retrun buffer which contains the json file
}

void parseString(char *doc, JSON *json, int * tokenIndex, int * pos) {
        char *begin = doc + *pos + 1;
        char *end = strchr(begin, '"');
        if (end == NULL)
            exit(1);
        int tokenlength = end - begin;
        json->tokens[(*tokenIndex)].start = *pos + 1;
        json->tokens[(*tokenIndex)].end = *pos + tokenlength;
        json->tokens[(*tokenIndex)].type = TOKEN_STRING;
        (*tokenIndex)++;
        *pos = *pos + tokenlength + 1;
}

void parseNumber(char *doc, JSON *json, int * tokenIndex, int * pos){
            char *begin = doc + *pos;
            char *end;
            char *buffer;
            int i = 0;

            end = strchr(doc + (*pos), ',');
            if (end == NULL)
            {
                // } 가 나오면 문자열이 끝남
                end = strchr(doc + (*pos), '}');
                if (end == NULL)
                    exit(1);
            }

            int tokenlength = end - begin;
            buffer = malloc(tokenlength + 1);
            memset(buffer, 0, tokenlength + 1);
            memcpy(buffer, begin, tokenlength);

            json->tokens[(*tokenIndex)].start = *pos;
            json->tokens[(*tokenIndex)].end = *pos + tokenlength;

            for (i = 0; i < strlen(buffer); i++)
            {
                if (buffer[i] == '.')
                    break;
            }
            if (i == strlen(buffer))
            {
                // int
                json->tokens[(*tokenIndex)].type = TOKEN_PRIMITIVE;
            }
            else
            {
                // float
                json->tokens[(*tokenIndex)].type = TOKEN_PRIMITIVE;
            }

            free(buffer);

            (*tokenIndex)++;

            *pos = *pos + tokenlength + 1;

}

void parseJSON(char *doc, int size, JSON *json) // JSON 파싱 함수
{
    int tokenIndex = 0; // token idx
    int pos = 0;        // position for traversing doc

    // if doc dosn't start with '{', it's invalid json file
    // return
    if (doc[pos] != '{')
        return;

    pos++; // go to next char which comes after '{'

    while (pos < size)
    {
        switch (doc[pos])
        {

        // strings
        case '"':
        {
            parseString(doc, json, &tokenIndex, &pos);
            break;
        }

        // string array
        case '[':
        {
            pos++;
            int arrcnt = 0;
            while (doc[pos] != ']') // 닫는 ]가 나오면 반복 종료
            {
                if (doc[pos] == '"') // 문자가 "이면 문자열
                {
                    json->tokens[tokenIndex].isArray = true;
                    parseString(doc, json, &tokenIndex, &pos);
                    arrcnt++;
                    json->tokens[tokenIndex - arrcnt - 1].size++;
                    // json->tokens[tokenIndex].isArray = 1;
                    json->tokens[tokenIndex - arrcnt - 1].isArraytitle = 1;
                }
                pos++; // 다음 문자로
            }
            break;
        }

            // numbers
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5': // 문자가 숫자이면
        case '6':
        case '7':
        case '8':
        case '9':
        case '-': // -는 음수일 때
        {
            parseNumber(doc, json, &tokenIndex, &pos);
            break;
        }
        }

        pos++; // 다음 문자로

        // control size when it's pair
        if (doc[pos] == ':')
        {
            json->tokens[--tokenIndex].size++;
            tokenIndex++;
        }
    }
    totaltokensize = tokenIndex;
}
void printtokens(JSON *json, char * doc) {
    int tokenindex = 0;
    int i = 0;
    int j = 0;

    for(tokenindex = 0; tokenindex < totaltokensize; tokenindex++) {
        // string case
        if (json->tokens[tokenindex].type == TOKEN_STRING) {
            if (json->tokens[tokenindex].isArray == 1) printf("\t");
            for(i  = json->tokens[tokenindex].start; i <= json->tokens[tokenindex].end; i++) {
                printf("%c", doc[i]);
            }
        }
        
        // primitive case
        else if (json->tokens[tokenindex].type == TOKEN_PRIMITIVE) {
            for(i  = json->tokens[tokenindex].start; i < json->tokens[tokenindex].end; i++) {
                printf("%c", doc[i]);
            }
        }
        printf("\n");
    }

}


int main()
{
    int size;

    // assign file content to one big string named doc
    // get size of file
    char *doc = readFile("example4.json", &size);
    if (doc == NULL)
        return -1;

    JSON json = {
        0,
    };

    parseJSON(doc, size, &json);
    printtokens(&json, doc);

    // free char pointer
    free(doc);

    return 0;
}