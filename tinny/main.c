#include "globals.h"

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
//Include code header files
#include "analyze.h"
#include "cgen.h"
#include "parse.h"
#include "scan.h"
#include "symtab.h"
#include "util.h"

#include "code.h"
/*
 * Flags NO_PARSE, NO_ANALYZE , NO_CODE
 */
int NO_PARSE=TRUE;
int NO_ANALYZE=TRUE;
int NO_CODE=TRUE;
//Options
int HELP=FALSE;
int COMPILE=FALSE;
int NO_OPTION=FALSE;
int UNDEFINED_OPTION=FALSE;
//Global variables
FILE *source;
FILE*listing;
FILE*code;
int lineno=0;
int EchoSource=TRUE;
int TraceScan=TRUE;
int TraceParse=TRUE;
int TraceAnalysis=TRUE;
int TraceCode=TRUE;
int Error=TRUE;

void printHelp();
//check is the flag to check
void parseArgs(int argc,char *argv[]){
    int opt;
	printf("Argument count %d\n",argc);
    while((opt=getopt(argc,argv,"hfnpac:"))!=-1){
	printf("Option: %c \n",opt);
        switch(opt){
        case 'h':
            HELP=TRUE;
            return;
        case 'f':
            COMPILE=TRUE;
        case 'n':
            NO_OPTION=TRUE;
            break;
        case 'p':
            if (NO_OPTION){
                NO_PARSE=FALSE;
                NO_OPTION=FALSE;
            }
            break;
        case 'a':
            if(NO_OPTION){
                NO_ANALYZE=FALSE;
                NO_OPTION=FALSE;
            }
            break;
        case 'c':
            if(NO_OPTION){
                NO_CODE=FALSE;
                NO_OPTION=FALSE;
            }
            break;
	case '?':
		break;
        default:
            printf("Undefined option %c in argument list \n",opt);
            UNDEFINED_OPTION=TRUE;
            return;

        }
    }
}
int main(int argc,char *argv[])
{
    parseArgs(argc,argv);
    if(UNDEFINED_OPTION){
        printHelp();
        exit(1);
    }

    TreeNode *syntaxTree;
    if(COMPILE){
        char pgm[20];
        strcpy(pgm,argv[argc-1]);
        if(strchr(pgm,'.')==NULL)
            strcat(pgm,".tny");
        source=fopen(pgm,"r");
        if(source==NULL){
            fprintf(stderr,"File %s not found\n",pgm);
            exit(1);
        }
        listing=stdout;
        fprintf(listing,"     TINY COMPILER       \n");
        if(!NO_PARSE){
	fprintf(listing,"---------Parsing------\n");
		TokenType token;
            while((token=getToken())!=ENDFILE){
			if(token==ERROR){
				fprintf(listing,"Error scanning file");
				break;
			}
		}
        }else{
            //syntaxTree=parse();
        if(!NO_ANALYZE){
            //buildSymbolTable(syntaxTree);
            //typeCheck(syntaxTree);
        }
        if(!NO_CODE){
            int slen=strlen(pgm);
            char *codefile=(char*)calloc(slen+4,sizeof(char));
            strncpy(codefile,pgm,slen);
            strcat(codefile,".tm");
            code=fopen(codefile,"w");
            if(code==NULL){
                fprintf(listing,"Unable to open %s \n",codefile);
                exit(1);
            }
            //codeGenerate(syntaxTree,codefile);
            fclose(code);
        }
	}
    } else if(HELP){
        printHelp();
    }else{
        printf("Error: Expected file or arguments\n");
        printHelp();
    }
    return 0;
}

void printHelp(){
    printf("The following functions and arguments are allowed\n");
    printf("\t tinny -f <file_name> -This option compiles the souce code from source to code generation.\n");
    printf("\t tinny [-np| --no-parse] -f <file_name>- This option allows the compiler to do only scanning.\n");
    printf("\t tinny [-na| --no-analyze] -f <file_name> -This option does scanning and parsing only.\n");
    printf("\t tinny [-nc| --no-code] -f <file_name> -This option does scanning, parsing, and code analysis but does not generate code.\n");
    printf("\t tinny [-es| --echo-source] -f <file_name> -This option prints source code on the terminal before compilation.\n");
    printf("\t tinny [-ts| --trace-scan] -f <file_name> -This option allows you to trace the scanning process. \n");
    printf("\t tinny [-tp| --trace-parse] -f <file_name> -This option allows traces the parsing process.\n");
    printf("\t tinny [-ta| --trace-analyze] -f <file_name> -This option traces the analyzer as it analyzes code. \n");
    printf("\t tinny [-tc| --trace-code] -f <file_name> -This option traces the code generation process.\n");
    printf("\t tinny [-h| --help] -Prints this help\n");
}

