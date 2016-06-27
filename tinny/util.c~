#include "globals.h"
#include "util.h"

void printToken(TokenType token, const char *tokenString){
    switch(token){
    case IF:
    case ELSE:
    case REPEAT:
    case END:
    case READ:
    case WRITE:
    case UNTIL:
    case FUNCTION:
        fprintf(listing,"Reserved word %s\n",tokenString);
        break;
    case ASSIGN:
        fprintf(listing,":=\n");
        break;
    case EQ:
        fprintf(listing,"=\n");
        break;
    case LT:
        fprintf(listing,"<\n");
        break;
    case LPAREN:
        fprintf(listing,"(\n");
        break;
    case RPAREN:
        fprintf(listing,")\n");
        break;
    case STMT:
        fprintf(listing,";\n");
        break;
    case PLUS:
        fprintf(listing,"+\n");
        break;
    case TIMES:
        fprintf(listing,"*\n");
        break;
    case OVER:
        fprintf(listing,"/\n");
        break;
    case ENDFILE:
        fprintf(listing,"<<EOF\n");
        break;
   case NUM:
        fprintf(listing,"NUM, val=%s\n",tokenString);
        break;
    case ID:
        fprintf(listing,"ID, name=%s\n",tokenString);
        break;
    case ERROR:
        fprintf(listing,"ERROR: %s\n",tokenString);
        break;
    default:
        fprintf(listing,"Unknown token: %d\n",token);
    }
}
TreeNode*newStatementNode(StatementKind kind){
    TreeNode *t=(TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d \n",lineno);

    }else{
        for(i=0;i<MAX_CHILDREN;i++)t->child[i]=NULL;
        t->siblings=NULL;
        t->nodeKind=StatementK;
        t->kind.expr=kind;
        t->lineno=lineno;
    }
}

TreeNode* newExpression(ExpressionKind kind){
    TreeNode *t=(TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d \n",lineno);

    }else{
        for(i=0;i<MAX_CHILDREN;i++)t->child[i]=NULL;
        t->siblings=NULL;
        t->nodeKind=ExpressionK;
        t->kind.expr=kind;
        t->lineno=lineno;
        t->type=Void;
    }
}
char* copyString(char * str){
    int n;
    char *t;
    if(str==NULL)
        return NULL;
    n=strlen(str);
    t=(char*)malloc(n*sizeof(char));
    if(t==NULL){
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    }else{
        strcpy(str,t);
    }
    return t;
}
