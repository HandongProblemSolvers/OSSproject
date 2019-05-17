// #define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>   // 파일 처리 함수가 선언된 헤더 파일
#include <stdlib.h>  // malloc, free 함수가 선언된 헤더 파일
#include <stdbool.h> // bool, true, false가 정의된 헤더 파일
#include <string.h>  // strchr, memset, memcpy 함수가 선언된 헤더 파일

// 토큰 종류 열거형
typedef enum _TOKEN_TYPE
{
    TOKEN_STRING, // 문자열 토큰
    TOKEN_NUMBER, // 숫자 토큰
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
    TOKEN_TYPE type;   // 토큰 종류
    union {            // 두 종류 중 한 종류만 저장할 것이므로 공용체로 만듦
        char *string;  // 문자열 포인터
        double number; // 실수형 숫자
    };
    bool isArray; // 현재 토큰이 배열인지 표시
    /*
    int start;
    int end;
    int size;
    */
} TOKEN;

#define TOKEN_COUNT 20 // 토큰의 최대 개수

// JSON 구조체
typedef struct _JSON
{
    TOKEN tokens[TOKEN_COUNT]; // 토큰 배열
} JSON;

char * readFile(char *filename, int *readSize) // 파일을 읽어서 내용을 반환하는 함수
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return NULL;

    int size;
    char *buffer;

    // get file size
    fseek(fp, 0, SEEK_END);     // move file pointer from 0 to end fo file
    size = ftell(fp);           // get size of file
    fseek(fp, 0, SEEK_SET);     // move file pointer back to 0

    buffer = malloc(size + 1);      // allcate file size + 1 buffer
    memset(buffer, 0, size + 1);    // init buffer with 0

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

    return buffer;  // retrun buffer which contains the json file
}

void parseJSON(char *doc, int size, JSON *json) // JSON 파싱 함수
{
    int tokenIndex = 0;     // token idx
    int pos = 0;            // position for traversing doc

    // if doc dosn't start with '{', it's invalid json file
    // return
    if (doc[pos] != '{')
        return;

    pos++;  // go to next char which comes after '{'

    while (pos < size)
    {
        switch (doc[pos]) // 문자의 종류에 따라 분기
        {
        case '"': // 문자가 "이면 문자열
        {
            // after reading '"', move position
            char *begin = doc + pos + 1;

            // get end position by finding second coming '"' char
            char *end = strchr(begin, '"');
            if (end == NULL) // "가 없으면 잘못된 문법이므로
                break;       // 반복을 종료

            // the length of contenct between first '"' and second '"' will be end-begin
            int stringLength = end - begin;

            // 토큰 배열에 문자열 저장
            // 토큰 종류는 문자열
            json->tokens[tokenIndex].type = TOKEN_STRING;
            // 문자열 길이 + NULL 공간만큼 메모리 할당
            json->tokens[tokenIndex].string = malloc(stringLength + 1);
            // 할당한 메모리를 0으로 초기화
            memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

            // 문서에서 문자열을 토큰에 저장
            // 문자열 시작 위치에서 문자열 길이만큼만 복사
            memcpy(json->tokens[tokenIndex].string, begin, stringLength);

            tokenIndex++; // 토큰 인덱스 증가

            pos = pos + stringLength + 1; // 현재 위치 + 문자열 길이 + "(+ 1)
        }
        break;
        }

        pos++; // 다음 문자로
    }
}

void freeJSON(JSON *json) // JSON 해제 함수
{
    for (int i = 0; i < TOKEN_COUNT; i++) // 토큰 개수만큼 반복
    {
        if (json->tokens[i].type == TOKEN_STRING) // 토큰 종류가 문자열이면
            free(json->tokens[i].string);         // 동적 메모리 해제
    }
}

int main()
{
    int size; // 문서 크기

    // assign file content to one big string named doc
    // get size of file
    char *doc = readFile("example.json", &size);
    if (doc == NULL)
        return -1;

    // allocate & init json struct
    JSON json = { 0, };

    // parse doc (which is same as input json file)
    parseJSON(doc, size, &json);

    printf("Title: %s\n", json.tokens[1].string);    // 토큰에 저장된 문자열 출력(Title)
    printf("Genre: %s\n", json.tokens[3].string);    // 토큰에 저장된 문자열 출력(Genre)
    printf("Director: %s\n", json.tokens[5].string); // 토큰에 저장된 문자열 출력(Director)

    // free json struct
    freeJSON(&json);

    // free char pointer 
    free(doc);

    return 0;
}