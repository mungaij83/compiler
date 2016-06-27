#ifndef SCAN_C
#define SCAN_C
#include "globals.h"
#include "scan.h"
#include "util.h"
//states in scanner DFA
typedef enum{START,IN_ASSIGN,IN_COMMENT,IN_NUM,IN_ID,DONE,IN_FUNCTION}StateType;
#define BUFF_LEN 256
static char lineBuff[BUFF_LEN];
char tokenString[MAX_TOKEN_LEN+1];
static int linepos=0; //Position in line buffer
static int buffsize=0;//Current size of buffer string
static char getNextChar(){
    if(!(linepos<buffsize)){
        lineno++;
        if(fgets(lineBuff,BUFF_LEN-1,source)){
            if(EchoSource)
                fprintf(listing,"%d: %s",lineno,lineBuff);
            buffsize=strlen(lineBuff);
            linepos=0;
            return lineBuff[linepos++];
        }else{
            return EOF;
        }
    }else{
        return lineBuff[linepos++];
    }
}
static void ungetNextChar(){
    if(linepos>0)
        linepos--;
}
//lookup table for reserved words
static struct {char *str;
               TokenType token;}
reservedWord[MAX_RESERVED_WORDS]={
    {"if",IF},
    {"else",ELSE},
    {"then",THEN},
    {"end",END},
    {"repeat",REPEAT},
    {"function",FUNCTION},
    {"until",UNTIL},
    {"read",READ},
    {"write",WRITE}};
static TokenType reservedLookup(char *s){
    int i;
    for(i=0;i<MAX_RESERVED_WORDS;i++){
        if(strcmp(s,reservedWord[i].str)==0){
            return reservedWord[i].token;
        }
    }
    return ID;
}

TokenType getToken(){
    int tokenindex=0;
    TokenType currentToken;
    StateType state=START;
    char c;
    int save;
    while(state!=DONE){
        c=getNextChar();
        switch(state){
        case START:
            save=TRUE;
		//fprintf(listing,"-----start, token=\' %c \' \n",c);
            if(isdigit(c)){
                state=IN_NUM;
            }else if(isalpha(c)){
                state=IN_ID;
            }else if(c==' ' || c=='\n'||c=='\t'){
                save=FALSE;
            }else if(c==':'){
                currentToken=IN_ASSIGN;
            }else if(c=='{'){
                save=FALSE;
                state=IN_COMMENT;
            }else{
		//fprintf(listing,"----------other, token=\'%c\'---------\n",c);
                state=DONE;
                switch(c){
                case EOF:
                    save=FALSE;
                    currentToken=ENDFILE;
                    break;
                case '=':
                    currentToken=EQ;
                    break;
                case '<':
                    currentToken=LT;
                    break;
                case '+':
                    currentToken=PLUS;
                    break;
                case '-':
                    currentToken=MINUS;
                    break;
                case '/':
                    currentToken=OVER;
                    break;
                case '*':
                    currentToken=TIMES;
                    break;
                case '(':
                    currentToken=LPAREN;
                    break;
                case ')':
                    currentToken=RPAREN;
                    break;
                case ';':
                    currentToken=STMT;
			//fprintf(listing,"------Statement---------\n");
                    break;
                default:
			//fprintf(listing,"error here\n");
                    currentToken=ERROR;
                    break;
                }
            }
            break;
        case IN_ASSIGN:
            state=DONE;
            if(c=='='){
                currentToken=ASSIGN;
            }else{
		//fprintf(listing,"error here\n");
                ungetNextChar();
                save=FALSE;
                currentToken=ERROR;
            }
            break;
        case IN_COMMENT:
            save=FALSE;
            if(c=='}'){
                state=START;
            }
            break;
        case IN_ID:
            if(!(isalpha(c)||isdigit(c))){
                ungetNextChar();
                save=FALSE;
                state=DONE;
                currentToken=ID;
		//fprintf(listing,"ID\n");
            }
            break;
        case IN_NUM:
            if(!isdigit(c)){
                ungetNextChar();
                save=FALSE;
                state=DONE;
                currentToken=NUM;
            }
            break;
	break;
        case DONE:
        default:
            fprintf(listing,"Scanner Bug: state=%d \n",state);
            state=DONE;
            currentToken=ERROR;
		break;
        }
    if(save && (tokenindex<=MAX_TOKEN_LEN)){
	//fprintf(listing,"save token \'%c\' \n",c);
        tokenString[tokenindex++]=c;
    }
    if(state == DONE){
        tokenString[tokenindex++]='\0';
        if(currentToken==ID){
            currentToken=reservedLookup(tokenString);
        }
    }
}
    if(TraceScan){
        fprintf(listing,"\t%d: ",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
}

#endif // SCAN_C
