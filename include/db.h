/**@<db.h>::**/
#pragma once
#include <constants.h> 
#include <stdbool.h>
/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 *************************************************/

/****************************************************************************
 * SYMTAB is an array used to store the name and position of each variable
 ****************************************************************************/
typedef struct _symtab_
{
	char name[MAXIDLEN + 1];
	int pos;
} SYMTAB;

typedef struct _getresponse_
{
	bool found;
	double value;
} GET_RESPONSE;

char error[100];

double memory[MAXSTBENTRIES];

SYMTAB symtab[MAXSTBENTRIES];
int symtab_nextentry;

GET_RESPONSE* get(const char *name);
void save(const char *name, double value);