/* front.c - a lexical analyzer system for simple
             arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

//Global variables
int charClass;
char lexeme [100];
int nextCharInt;
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

// Function Declarations
void expr();
void term();
void factor();
void addChar();
void getChar();
void getNonBlank();
int lex();
int indentifier();
//Character classes
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

//Token codes
#define ID 8
#define KEYWORD 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define FOR_CODE 30
#define IF_CODE 31
#define ELSE_CODE 32
#define WHILE_CODE 33
#define DO_CODE 34
#define INT_CODE 35
#define FLOAT_CODE 36
#define SWITCH_CODE 37

#define MAX_RESERVED_WORDS 8
//Create a lookup table for key words
typedef enum{FOR_C,IF_C,ELSE_C,WHILE_C,DO_C,INT_C,FLOAT_C,SWITCH_C}TokenType;
static struct {char *str;
               TokenType token;}
              reservedWord[MAX_RESERVED_WORDS]={
{"if",IF_C},
{"else",ELSE_C},
{"while",WHILE_C},
{"do",DO_C},
{"switch",SWITCH_C},
{"float",FLOAT_C},
{"for",FOR_C},
{"int",INT_C}};
static int reservedLookup(char *s){
    int i;
    int type;
    for(i=0;i<MAX_RESERVED_WORDS;i++){
        if(strcmp(s,reservedWord[i].str)==0){
            type= reservedWord[i].token;
            switch(type){
            case IF_C:
                return IF_CODE;
            case ELSE_C:
                return ELSE_CODE;
            case WHILE_C:
                return WHILE_CODE;
            case DO_C:
                return DO_CODE;
            case SWITCH_C:
                return SWITCH_CODE;
            case INT_C:
                return INT_CODE;
            case FLOAT_C:
                return FLOAT_CODE;
            default:
                //This will never happen
                printf("Error");
            }
        }
    }
    return ID;
}
//Main
int main(){
    if ((in_fp = fopen("front.txt", "r")) == NULL)
        printf("ERROR - cannot open front.txt \n");
    else{
        getChar();
        do {
            lex();
            expr();
        } while (nextToken != EOF);
    }
}


void expr() {
    printf("Enter <expr>\n");
    term();

    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        lex();
        term();
    }
    printf("Exit <expr>\n");
}  /* End expr() */

void term() {
    printf("Enter <term>\n");
    factor();

    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
    printf("Exit <term>\n");
} /* End term() */

void factor() {
    printf("Enter <factor>\n");

    if (nextToken == IDENT || nextToken == INT_LIT)
        lex();

    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
            else
                printf("Error");
        }

        else
            printf("Error");
    }

    printf("Exit <factor>\n");
} /* End factor() */
int indentifier(){
    lexLen=0;
    int index=0;
    char buffer[256];
    buffer[index]=nextChar;
    int tokenType;
    while(charClass!=IDENT){
        if(charClass==LETTER||nextChar==DIGIT){
            buffer[index++]=nextChar;
            getChar();
            addChar();
        }else{
            return UNKNOWN;
        }
    }
    tokenType=reservedLookup(buffer);
    switch(tokenType){
    case ID:
        return ID;
    case FLOAT_CODE:
    case INT_CODE:
    case WHILE_CODE:
    case DO_CODE:
    case SWITCH_CODE:
    case IF_CODE:
    case ELSE_CODE:
    case FOR_CODE:
        return KEYWORD;
    default:
        return UNKNOWN;
    }
}
// lookUp
int lookup(char ch){
    switch(ch){
    case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;

    case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;

    case '+':
        addChar();
        nextToken = ADD_OP;
        break;

    case'-':
        addChar();
        nextToken = SUB_OP;
        break;

    case'*':
        addChar();
        nextToken = MULT_OP;
        break;

    case'/':
        addChar();
        nextToken = DIV_OP;
        break;

    default:
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

//AddChar
void addChar(){
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

//getChar
void getChar(){
    if ((nextCharInt = getc(in_fp)) != -1) {
        nextChar=(char)nextCharInt;
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}

// getNonBlank
void getNonBlank(){
    while (isspace(nextChar))
        getChar();
}

//lex
int lex(){
    int tokenType;
    lexLen = 0;
    getNonBlank();
    switch (charClass){
    case LETTER:
        tokenType=indentifier();
        if(tokenType==UNKNOWN){
            nextToken=UNKNOWN;
            break;
        }
        //Determine token type
        nextToken = IDENT;
        break;
        // parse int lits
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT){
            addChar();
            getChar();
        }
        nextToken = INT_LIT;
        break;

        //pares and ops
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

        //EOF
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    }
    // end of switch
    printf("Next token is: %d, next lexeme is %s\n",nextToken, lexeme);
    return nextToken;
}

