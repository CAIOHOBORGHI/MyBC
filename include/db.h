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

/*****************************************************************************
 * STRUCT GET_RESPONSE
 * 
 * Is used because without the flag 'found' we would not be able to know if 
 * user entered an unregistered variable(because the value would it be 0)
 ******************************************************************************/
typedef struct _getresponse_
{
	bool found;
	double value;
} GET_RESPONSE;

// Variables declared in db.c
extern char error[100];
extern double memory[MAXSTBENTRIES];
extern SYMTAB symtab[MAXSTBENTRIES];
extern int symtab_nextentry;

// Memory-Database Methods
GET_RESPONSE *get(const char *name);
void save(const char *name, double value);