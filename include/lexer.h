/**@<lexer.h>::**/
#pragma once
/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 *************************************************/
#include <tokens.h>
#include <constants.h>
#include <keywords.h>

extern char lexeme[MAXIDLEN + 1];
int gettoken(FILE *source);