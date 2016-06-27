#ifndef SCAN_H
#define SCAN_H
#include "globals.h"

#define MAX_TOKEN_LEN 40
extern char tokenString[MAX_TOKEN_LEN+1];
TokenType getToken();

#endif // SCAN_H
