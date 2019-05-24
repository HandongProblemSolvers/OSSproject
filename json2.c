#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char * readfile(char * , int * );
void startParse(int );
int parseString(int );
// int parseArray(int );
int parseObject(int );

typedef enum {
    UNDEFINED,
    OBJECT,
    ARRAY,
    STRING,
    PRIMITIVE
} token_t;

typedef struct {
    token_t type;
    int start;
    int end;
    int size;
} token;

token tokens[255];
char * file;
int totaltokensize = 0;
int tokenidx = 0;

int main (int argc, char* argv[]) {
    int filesize = 0;
    int i = 0;
    int j = 0;

    printf("filename : %s\n", argv[1]);
    char * filename = argv[1];
    
    file = readfile(filename, &filesize);
    if (file == NULL) return -1;

    startParse(filesize);
    
    for(i = 0; i < totaltokensize; i++) {
        printf("[ %d] ", i+1);
        for(j = tokens[i].start; j < tokens[i].end; j++) {
            printf("%c", file[j]);
        }
        printf(" (size=%d, %d~%d, JSMN_%u))", tokens[i].size, tokens[i].start, tokens[i].end, tokens[i].type);
        printf("\n");
    }
    return 0;
}

void startParse(int filesize) {
    int i = 0;
    int pos = 0;

    if (file[pos] != '{') return ;
    pos++;
    while(pos < filesize){
        switch(file[pos]) {

            case '{' : {
                pos = parseObject(pos);
                break;
            }
            case '"' : {
                // parse string
                // until meeting second ", it's one token
                pos = parseString(pos);
                break;
            }
            case '[' : {
                // parse array
                // pos = parseArray(pos);
                break;
            }
            case ':' : {
                tokens[--tokenidx].size++;
                tokenidx++;
            }
        }   // end of switch
        pos++;
    }   // end of for loop
}

int parseObject(int pos) {
    int initpos = pos;
    int endpos = 0;
    int cnt = 0;
    pos++;
    tokens[tokenidx].size = 1;
    while(1) {
        if( (file[pos] == '}') && (cnt == 0) ) break; 
        if(file[pos] == '{') cnt++;
        if(file[pos] == '}') cnt--;
        if(file[pos] == ',') tokens[tokenidx].size++;
        pos++;
    }
    tokens[tokenidx].type = OBJECT;
    tokens[tokenidx].start = initpos;
    tokens[tokenidx].end = pos + 1;
    tokenidx++;
    totaltokensize++;
    return initpos + 1;
}

int parseString(int pos) {
    pos++;
    tokens[tokenidx].type = STRING;
    tokens[tokenidx].start = pos;
    while(file[pos] != '"'){
        pos++;
    }
    tokens[tokenidx].end = pos;
    tokenidx++;
    totaltokensize++;
    return pos;
}
// int parseArray(int pos) {
//     pos++;
//     while(file[pos] != ']') {
//         switch(file[pos]) {
//             case '"' : {
//                 pos = parseString(pos);
//                 break;
//             }
//             case '{' : {
//                 pos = parseObject(pos);
//                 break;
//             }
//         }
//         pos++;
//     }
//     return pos;
// }
char * readfile(char * filename, int * filesize) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    int size = 0;
    char * buffer;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(size + 1);
    memset(buffer, 0, size + 1);

    if(fread(buffer, size, 1, fp) < 1) {
        *filesize = 0;
        free(buffer);
        fclose(fp);
        return NULL;
    }
    *filesize = size;
    fclose(fp);
    return buffer;
}