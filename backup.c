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
void printNewWindow();
void printClientMenu();
void randomMatchClientVer();
void showClientAge();
void showClientCompany();
void showClientEmail();
void showClientPhoneNum();

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
    char isAdmin = 0;
    char password[] = "loveisanopendoor";
    char inputpassword[20];

    printf("filename : %s\n", argv[1]);
    char * filename = argv[1];
    
    file = readfile(filename, &filesize);
    if (file == NULL) return -1;

    startParse(filesize);
    printNewWindow();
    scanf("%c", &isAdmin);
    if(tolower(isAdmin)=='a'){
        printf("Enter Password: ");
        scanf("%s",inputpassword);
        if(strcmp(inputpassword, password)){
            printf("You Entered Wrong Password!!!!!\n");
            exit(0);
        }else{
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
                    case 4: 
                        seeClientbyGender();
                        break;
                    case 5:
                        seeClientbyHobby();
                        break;
                    case 6: 
                        seeClientbyLocalInfo();
                        break;
                    case 7: 
                        seeClientbyCommpany();
                        break;
                    case 8:
                        seeClientbyEmail();
                        break;
                    case 99 :
                        printf("- SYS END -\n");
                        return 0;
                    default :
                        wrongInput();
                }
            }
        }
    }else if(tolower(isAdmin)=='c'){
            int index;
            while(1){
                printClientMenu();
                printf("Input : ");
                scanf("%d",&index);
                switch(index){
                case 0 : 
                    randomMatchClientVer();
                    break;
                case 1 : 
                    showClientAge();
                    break;
                case 2 : 
                    showClientCompany();
                    break;
                case 3 :
                    showClientEmail();
                    break;
                case 4: 
                    showClientPhoneNum();
                    break;
                case 99 :
                    printf("- SYS END -\n");
                    return 0;
                default :
                    wrongInput();
            }
        }
    }else{
        wrongInput();
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
    while( (file[pos] != ']') && (file[pos] != ',') && (file[pos] != '}') && (file[pos] != ' ') && (file[pos] != '\n')) {
        pos++;
    }
    tokens[tokenidx].end = pos;
    tokenidx++;
    totaltokensize++;
    return pos;
}

int parseObject(int pos) {
    int initpos = pos;
    int cnt = 0;
    int flag = 0;
    pos++;
    tokens[tokenidx].size = 1;
    while(1) {
        if( (file[pos] == '}') && (cnt == 0) ) break; 
        if( (file[pos] == '{') || (file[pos] == '[') )  {
            cnt++;
            flag++;
        }
        if( (file[pos] == '}') || (file[pos] == ']') ) {
            cnt--;
            flag--;
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
    int cnt = 0;
    int flag = 0;
    pos++;
    tokens[tokenidx].size = 1;
    while(1) {
        if( (file[pos] == ']') && (cnt == 0) ) break; 
        if( (file[pos] == '[') || (file[pos] == '{') )  {
            cnt++;
            flag++;
        }
        if( (file[pos] == ']') || (file[pos] == '}') ) {
            cnt--;
            flag--;
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
    char * token_types[5] = {"UNDEFINED", "OBJECT", "ARRAY", "STRING", "PRIMITIVE"};
    for(int i = 0; i < totaltokensize; i++) {
        printf("[ %d] ", i+1);
        for(int j = tokens[i].start; j < tokens[i].end; j++)
            printf("%c", file[j]);
        printf(" (size=%d, %d~%d, JSMN_%s))\n", tokens[i].size, tokens[i].start, tokens[i].end, token_types[tokens[i].type]);
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
    printf("** NOT EXIST **\n");
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
    printf("** NOT EXIST **\n");
    printf("*****************\n");
}

void randomMatchClientVer(){
    //get conditions from user
    char privateNo[50];
    int cnt = 0;
    int cnt2=0;
    int this_age;
    bool exist = false;
    char this_gender[10];
    int expect_age;
    char expect_hobby[50];
    char clients_Age[50][256];
    char clients_filtered[50][256];
    int s=0;
    printf("type your private number: ");
    scanf("%s", privateNo);
    

    //if privateNo doesn't exist error!
    for(int i=0; i< totaltokensize; i++){
        if(mystrcmp("id",tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            if(mystrcmp(privateNo, tokens[i+1].start, tokens[i+1].end)){
               
                exist = true;
                break;
            }
        }
    }

    if(exist== false){
        printf("You're not a member of us! Join us!\n");
        return;
    }

    
 
    //get client info
    for(int i =0; i < totaltokensize; i++){
        if(mystrcmp("id", tokens[i].start, tokens[i].end) && tokens[i].size == 1 && mystrcmp(privateNo,tokens[i+1].start,tokens[i+1].end)){
            
            strcpy(this_gender, extractToken(tokens[i+7].start, tokens[i+7].end));
            
        }
    }
    printf("Under what age you want your partner to be?: ");
    scanf("%d", &expect_age);
    
    
    //filter clients with age
    for(int i=0; i< totaltokensize; i++){
      if(mystrcmp("age", tokens[i].start, tokens[i].end) && tokens[i].size == 1){
            if(atoi(extractToken(tokens[i+1].start, tokens[i+1].end)) <= expect_age && !mystrcmp(this_gender,tokens[i+5].start,tokens[i+5].end) && !mystrcmp(privateNo,tokens[i-1].start, tokens[i-1].end)){
              strcpy(clients_Age[s++],extractToken(tokens[i+3].start, tokens[i+3].end));
              cnt++;
            }
        }
    }

    fflush(stdin);
    printf("What kind of hobby you want your partner to have?: ");
    gets(expect_hobby);
   
   
    
    //filter 한 애들 대상으로 Hobby로 filter
    for(int i=0,k=0; i< cnt; i++){
        for(int j=0; j<totaltokensize; j++){
            if(mystrcmp("name", tokens[j].start,tokens[j].end) && tokens[j].size==1 && mystrcmp(clients_Age[i],tokens[j+1].start, tokens[j+1].end)){
                if(mystrcmp(expect_hobby, tokens[j+11].start, tokens[j+11].end)){
                 
                    strcpy(clients_filtered[cnt2],clients_Age[i]);
                    
                    cnt2++;
                }
            }
        }

    }

  

        if(cnt2==0){
            printf("Sorry No Match... Why don't you lower your expectations?\n");
            fflush(stdin);
            return;
        }else if(cnt2==1){
            printf("--------------Matching Result-------------------\n");
            printf("%s\n", clients_filtered[cnt2-1]);
            fflush(stdin);
            return;
        }else if(cnt>1){
            srand(time(NULL));
            printf("--------------Matching Result-------------------\n");
            printf("%s\n", clients_filtered[rand()%cnt2]);
            fflush(stdin);
            return;
        }

    

    }


    void showClientAge(){
    char getname[50];
    printf("Type the name of a person you want to know about his/her age(Case Sensitive)\n");
    fflush(stdin);
    gets(getname);
    for(int i=0; i<totaltokensize;i++){
        if(mystrcmp("name", tokens[i].start, tokens[i].end)&& tokens[i].size ==1 ){
            
            if(mystrcmp(getname, tokens[i+1].start,tokens[i+1].end)){
            printf("%s's age is %s years old\n", extractToken(tokens[i+1].start, tokens[i+1].end), extractToken(tokens[i-1].start,tokens[i-1].end));
            return;
            }
        }
    }
    printf("There's no such name as %s in our database, please check the spelling(Case Sensitive)\n",getname);
    return;
}

void showClientCompany(){
    char getname[50];
    printf("Type the name of a person you want to know about his/her job\n");
    fflush(stdin);
    gets(getname);
    for(int i=0; i<totaltokensize;i++){
        if(mystrcmp("name", tokens[i].start, tokens[i].end) && tokens[i].size==1){
            if(mystrcmp(getname, tokens[i+1].start, tokens[i+1].end)){
                printf("%s's company is %s!\n", getname,extractToken(tokens[i+5].start, tokens[i+5].end));
                return;
            }
        }
    
    }
    printf("There's no such name as %s in out database, please check the spelling\n",getname);
    return;
}

void showClientEmail(){
    char getname[50];
    printf("Type the name of a person you want to know about his/her Email(Case Sensitive)\n");
    fflush(stdin);
    gets(getname);
    for(int i=0; i<totaltokensize;i++){
        if(mystrcmp("name", tokens[i].start, tokens[i].end) && tokens[i].size==1){
            if(mystrcmp(getname, tokens[i+1].start, tokens[i+1].end)){
                printf("%s's email is %s!\n", getname,extractToken(tokens[i+7].start, tokens[i+7].end));
                return;
            }
        }
    
    }
    printf("There's no such name as %s\n",getname);
    return;
}

void showClientPhoneNum(){
    char getname[50];
    printf("Type the name of a person you want to know about his/her Phone Number(Case Sensitive)\n");
    fflush(stdin);
    gets(getname);
    for(int i=0; i<totaltokensize;i++){
        if(mystrcmp("name", tokens[i].start, tokens[i].end) && tokens[i].size==1){
            if(mystrcmp(getname, tokens[i+1].start, tokens[i+1].end)){
                printf("%s's phne number is %s!\n", getname,extractToken(tokens[i+9].start, tokens[i+9].end));
                return;
            }
        }
    
    }
    printf("There's no such name as %s\n",getname);
    return;
}

void printNewWindow(){
    printf("------------Welcome to loveisabuisness.com!-------------\n");
    printf("If you are administrator press 'A'\n");
    printf("If you are not, just press 'C' to continue\n");
}

void printClientMenu(){
    printf("----------Welcome to www.loveisanopendoor.com !!!----------\n");
    printf("0. Random Matching: According to conditions\n");
    printf("1. Get Age Information of other Client\n");
    printf("2. Get Job Information of other Client\n");
    printf("3. Get Email of other Client\n");
    printf("4. Get Phone Number of other Client\n");
    printf("99. Quit\n"); 
    printf("----------------------------------------------\n");
}