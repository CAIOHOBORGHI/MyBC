/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>

extern char lexeme[];

float E(void);
float T(void);
float F(void);

void cmd(void);
int is_end(void);

void match(int expected);
int gettoken(FILE *);

float get(const char *name);
void save(const char *name, float value);
void setError(char *message);

extern int lookahead;
extern FILE *source;
