/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>

extern char lexeme[];

double E(void);
double T(void);
double F(void);

void cmd(void);
int is_end(void);

void match(int expected);
int gettoken(FILE *);

double get(const char *name);
void save(const char *name, double value);
void setError(char *message);

extern int lookahead;
extern FILE *source;
