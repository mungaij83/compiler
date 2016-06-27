#ifndef UTIL_H
#define UTIL_H
#include "globals.h"
/*
 *Conatin functions required to generate internal representation of the source code (syntax tree)
 * and display listing errors.
 */
//print a token and its lexeme to listing file
void printToken(TokenType,const char*);
//Creates a new statement node for syntax tree creation
TreeNode * newStatementNode(StatementKind);
//Creates a new expression node
TreeNode * newExpression(ExpressionKind);
//Allocates and makes a copy of new character string
char* copyString(char*);
//prints syntax tree to the listing file with idention
void printTree(TreeNode*);
#endif // UTIL_H
