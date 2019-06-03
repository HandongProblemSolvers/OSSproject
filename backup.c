#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define WINDOW
#ifdef WINDOW
#define FREAD(x, y, z, a) fread(x, z, y, a)
#endif

char * readfile(char * , int * );
void startParse(int );
int parseString(int );
int parseArray(int );
int parseObject(int );
int parsePrimitive(int );
int mystrcmp(char * , int , int );
char * extractToken(int , int );

//application function (TBD)
void printMenu();
void wrongInput();
void printTokens();
void randomMatching();
void seeClient();
void seeClientbyAge();
void seeClientbyGender();
void seeClientbyHobby();
void seeClientbyLocalInfo();
void seeClientbyCommpany();
void seeClientbyEmail();

typedef enum {
    UNDEFINED = 0,
    OBJECT = 1,
    ARRAY = 2,
    STRING = 3,
    PRIMITIVE = 4
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
    int c = 0;

    printf("filename : %s\n", argv[1]);
    char * filename = argv[1];
    
    file = readfile(filename, &filesize);
    if (file == NULL) return -1;

    startParse(filesize);
    
    while(1){
        printMenu();
        printf("Input : "); 
        scanf("%d", &c); getchar();
        
        switch(c){
            case 0 : 
                printTokens();
                break;
            case 1 : 
                randomMatching();
                break;
            case 2 : 
                seeClient();
                break;
            case 3 :
                seeClientbyAge();
                break;
            case 4 :
                seeClientbyGender();
                break;
            case 5 :
                seeClientbyHobby();
                break;
            case 6 : 
                seeClientbyLocalInfo();
                break;
            case 7 :
                seeClientbyCommpany();
                break;
            case 8 :
                seeClientbyEmail();
                break;
            case 99 :
                printf("- SYS END -\n");
                return 0;
            default :
                wrongInput();
        }
    }

    return 0;
}

int mystrcmp(char * s, int start, int end){
    if(strlen(s) != (end - start)) return 0;
    for(int i = 0; i < (end-start); i++)
        if(s[i] != file[start+i]) return 0;
    return 1;
}

char * extractToken(int start, int end){
    char * token = (char *)malloc(end - start + 1);
    for(int i = 0; i < end - start; i++)
        token[i] = file[start+i];
    token[end-start] = '\0';
    return token;
}

void startParse(int filesize) {
    int i = 0;
    int pos = 0;
    int temppos = 0;
    char * checkbool;
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
                pos = parseArray(pos);
                break;
            }
            case 't' : case 'f' : case 'n' : {
                // parse boolean and null
                temppos = pos;
                if (file[pos] == 't' || file[pos] == 'n') {
                    // true, null
                    checkbool = (char *)malloc(5);
                    strncpy(checkbool, file+pos, 4);
                    if( (strcmp(checkbool, "true") == 0) || (strcmp(checkbool, "null") == 0) ) {
                        pos = parsePrimitive(pos);
                    }
                    else break;
                    free(checkbool);
                } else {
                    // false
                    checkbool = (char *)malloc(6);
                    strncpy(checkbool, file+pos, 5);
                    if( strcmp(checkbool, "false") == 0 ) {
                        pos = parsePrimitive(pos);
                    }
                    else break;
                    free(checkbool);
                }
                break;
            }
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9': 
            case '-': {
                // parse numbers
                pos = parsePrimitive(pos);
                break;
            }
            case ':' : {
                tokens[--tokenidx].size++;
                tokenidx++;
                break;
            }
        }   // end of switch
        pos++;
    }   // end of for loop
}

int parsePrimitive(int pos) {
    tokens[tokenidx].type = PRIMITIVE;
    tokens[tokenidx].start = pos;
    while(file[pos] != ','){
        pos++;
    }
    tokens[tokenidx].end = pos;
    tokenidx++;
    totaltokensize++;
    return pos;
}

int parseObject(int pos) {
    int initpos = pos;
    int endpos = 0;
    int cnt = 0;
    int flag = 0;
    pos++;
    tokens[tokenidx].size = 1;
    while(1) {
        if( (file[pos] == '}') && (cnt == 0) ) break; 
        if( (file[pos] == '{') || (file[pos] == '[') )  {
            cnt++;
            flag = 1;
        }
        if( (file[pos] == '{') || (file[pos] == ']') ) {
            cnt--;
            flag = 0;
        }
        if( (file[pos] == ',') && (flag == 0) ) {
            tokens[tokenidx].size++;
        }
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

int parseArray(int pos) {
    int initpos = pos;
    int endpos = 0;
    int cnt = 0;
    int flag = 0;
    pos++;
    tokens[tokenidx].size = 1;
    while(1) {
        if( (file[pos] == ']') && (cnt == 0) ) break; 
        if( (file[pos] == '[') || (file[pos] == '{') )  {
            cnt++;
            flag = 1;
        }
        if( (file[pos] == ']') || (file[pos] == '}') ) {
            cnt--;
            flag = 0;
        }
        if( (file[pos] == ',') && (flag == 0) ) {
            tokens[tokenidx].size++;
        }
        pos++;
    }
    tokens[tokenidx].type = ARRAY;
    tokens[tokenidx].start = initpos;
    tokens[tokenidx].end = pos + 1;
    tokenidx++;
    totaltokensize++;
    return initpos + 1;
}

char * readfile(char * filename, int * filesize) {
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    int size = 0;
    char * buffer;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = (char *)malloc(size + 1);
    memset(buffer, 0, size + 1);

    // change this fread function's argument into fread(buffer, 1, size, fp) in Windows OS
    // if(fread(buffer, size, 1, fp) < 1) {
    //     *filesize = 0;
    //     free(buffer);
    //     fclose(fp);
    //     return NULL;
    // }
    if(FREAD(buffer, size, 1, fp) < 1) {
        *filesize = 0;
        free(buffer);
        fclose(fp);
        return NULL;
    }
    *filesize = size;
    fclose(fp);
    return buffer;
}

void printMenu(){
    printf("----------Marriage Management System----------\n");
    printf("0. Print All Parsing Data\n");
    printf("1. Random Matching\n");
    printf("2. See Client Information\n");
    printf("3. See Clients By Age Limit\n"); 
    printf("4. See Clients By Gender \n"); 
    printf("5. See Clients By Hobby\n");
    printf("6. See Clinents By Area Code\n");
    printf("7. See Clients By Company\n");
    printf("8. See Clients By Email\n");
    printf("99. Quit\n"); 
    printf("----------------------------------------------\n");
}

void wrongInput(){
    printf("*****************\n");
    printf("** WRONG INPUT **\n");
    printf("*****************\n");
}

void printTokens(){
    for(int i = 0; i < totaltokensize; i++) {
        printf("[ %d] ", i+1);
        for(int j = tokens[i].start; j < tokens[i].end; j++)
            printf("%c", file[j]);
        printf(" (size=%d, %d~%d, JSMN_%u))\n", tokens[i].size, tokens[i].start, tokens[i].end, tokens[i].type);
    }
}

void randomMatching(){
    int m = 0;
    int w = 0;
    int chk[128] = {0, };
    char key[] = "name";
    char sex[] = "male";
    char male[64][128], female[64][128];
    
    srand(time(NULL));

    for(int i = 0; i < totaltokensize; i++) {
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            if(mystrcmp(sex, tokens[i+3].start, tokens[i+3].end))
                strcpy(male[m++], extractToken(tokens[i+1].start, tokens[i+1].end));
            else 
                strcpy(female[w++], extractToken(tokens[i+1].start, tokens[i+1].end));
        }
    }
    printf("----------- MATCHING LIST -----------\n");
    for(int i = 0; i < MIN(w,m); i++){
        while(1){
            int index = rand() % MAX(w,m);
            if(chk[index]) continue;
            chk[index] = 1;
            if(m < w) printf("[  %s  VS  %s  ]\n", male[i], female[index]);
            else printf("[  %s  VS  %s  ]\n", female[i], male[index]);
            break;
        } 
    }
    printf("------------------------------------\n");
}

void seeClient(){
    char name[128] = {0, };
    printf("NAME : ");
    scanf("%[^\n]", name);
    char key[] = "name";
    for(int i = 0; i < totaltokensize; i++) {
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1 && mystrcmp(name, tokens[i+1].start, tokens[i+1].end)){
            printf("------------------- I N F O ------------------\n");
            for(int j = -2; j <= 12; j+=2)
                printf("%s : %s\n", extractToken(tokens[i+j].start, tokens[i+j].end), extractToken(tokens[i+j+1].start, tokens[i+j+1].end));
            printf("----------------------------------------------\n");
            return;
        }
    }
    printf("*****************\n");
    printf("** NOT EXISTED **\n");
    printf("*****************\n");
}



void seeClientbyAge(){
    int cnt = 0;
    int age_limit = 0;
    printf("AGE : ");
    scanf("%d", &age_limit);
    char key[] = "age";
    printf("--------------- UNDER AGE : %d --------------\n", age_limit);
    for(int i = 0; i < totaltokensize; i++) 
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            if(atoi(extractToken(tokens[i+1].start, tokens[i+1].end)) <= age_limit){
                printf("%s : %s years\n", extractToken(tokens[i+3].start, tokens[i+3].end), extractToken(tokens[i+1].start, tokens[i+1].end));
                cnt++;
            }
        }
    printf("THERE ARE %d PEOPLE\n", cnt);
    printf("----------------------------------------------\n");
}

void seeClientbyGender(){
    int cnt = 0;
    char gender[128] = {0,};
    printf("Gender : ");
    scanf("%[^\n]", gender);
    char key[] = "gender";

    for(int i = 0 ; gender[i] ; i++){
        gender[i] = tolower(gender[i]);
    }
    printf("--------------- GENDER : %s --------------\n",gender);
    for(int i = 0; i < totaltokensize; i++) {
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1 && mystrcmp(gender, tokens[i+1].start, tokens[i+1].end)){
            printf("%s : %s \n", extractToken(tokens[i-1].start, tokens[i-1].end), extractToken(tokens[i+1].start, tokens[i+1].end));
            cnt++;
        }
    }

    printf("\nTHERE ARE %d PEOPLE\n", cnt);
    printf("----------------------------------------------\n");
}

void seeClientbyHobby(){

    char key[] = "hobby";
    char hobby[64][128];
    int cnt=0;
    int isCheck=0;
    for(int i = 0; i < totaltokensize; i++) {
        isCheck=0;
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            for(int j = 0 ;j<cnt; j++){
               // printf("%s : %s \n",hobby[j],extractToken(tokens[i+1].start, tokens[i+1].end));
                if(strcmp(extractToken(tokens[i+1].start, tokens[i+1].end),hobby[j])==0){
                    isCheck=1;
                    break;
                }
            }
            if(isCheck==1) continue;
            strcpy(hobby[cnt++], extractToken(tokens[i+1].start, tokens[i+1].end));
            
        }
    }
   
    
    printf("\n");
    for(int i = 0 ; i<cnt ; i++){
        printf("--------------- HOBBY : %s -------------\n",hobby[i]);
        for(int j = 0; j < totaltokensize; j++) {
            if(mystrcmp(key, tokens[j].start, tokens[j].end) && tokens[j].size == 1 && mystrcmp(hobby[i],tokens[j+1].start, tokens[j+1].end)){
                printf("%s : %s \n", extractToken(tokens[j-9].start, tokens[j-9].end),extractToken(tokens[j-7].start, tokens[j-7].end));
                }
         }
    }
    printf("\n");

}

void seeClientbyLocalInfo(){
    int cnt =0;
    int area_code = 0;
    char key[] ="phone";
    printf("Area Code: ");
    scanf("%d",&area_code);
    printf("---------------- LOCAL NUMBER : %d ------------------\n", area_code);
    for(int i=0; i<totaltokensize;i++){
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            // printf("%s \n",extractToken(tokens[i+1].start, tokens[i+1].end));
            // printf("%s \n",extractToken(tokens[i-7].start, tokens[i-7].end));
            if(atoi(extractToken(tokens[i+1].start+4, tokens[i+1].start+7))==area_code){
                printf("AREA CODE: %s NAME: %s \n", extractToken(tokens[i+1].start+3,tokens[i+1].start+8),extractToken(tokens[i-7].start, tokens[i-7].end),extractToken(tokens[i+1].start, tokens[i+1].end));
                cnt ++;
            }
        }
    }

    printf("THERE ARE %d PEOPLE LIVE IN THE GIVEN AREA\n", cnt);
    printf("----------------------------------------------\n");
}

void seeClientbyCommpany(){
    int cnt = 0;
    char name[128] = {0, };
    printf("COMPANY : ");
    scanf("%[^\n]", name);
    for(int i = 0; i < strlen(name); i++)
        if(name[i] >= 'a' && name[i] <= 'z')
            name[i]= name[i] - 32;

    char key[] = "company";
    printf("--------------- COMPANY : %s --------------\n", name);
    for(int i = 0; i < totaltokensize; i++) 
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            if(mystrcmp(name, tokens[i+1].start, tokens[i+1].end))
                printf("%d. %s\n", ++cnt, extractToken(tokens[i-3].start, tokens[i-3].end));
        }
    printf("THERE ARE %d PEOPLE\n", cnt);
    printf("----------------------------------------------\n");
}

void seeClientbyEmail(){
    char name[128] = {0, };
    printf("E-MAIL : ");
    scanf("%[^\n]", name);
    char key[] = "email";
    for(int i = 0; i < totaltokensize; i++) {
        if(mystrcmp(key, tokens[i].start, tokens[i].end) && tokens[i].size == 1 && mystrcmp(name, tokens[i+1].start, tokens[i+1].end)){
            printf("------------------- I N F O ------------------\n");
            for(int j = -8; j <= 6; j+=2)
                printf("%s : %s\n", extractToken(tokens[i+j].start, tokens[i+j].end), extractToken(tokens[i+j+1].start, tokens[i+j+1].end));
            printf("----------------------------------------------\n");
            return;
        }
    }
    printf("*****************\n");
    printf("** NOT EXISTED **\n");
    printf("*****************\n");
}
