/**@<parser.h>::**/
/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 *************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>

int lookahead;
extern FILE *source;
extern char lexeme[];

double E(void);
double T(void);
double F(void);

void cmd(void);
int is_end(void);
void match_end(void);
int match_if_sep(void);
double dbop(void);

void match(int expected);
int gettoken(FILE *);
void setError(char *message);