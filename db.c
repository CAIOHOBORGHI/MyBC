/**@<db.c>::**/
#include <string.h>
#include <stdlib.h>
#include <db.h>

/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 *************************************************/

/****************************************************************************
 * This file works as a memory-database
 * 
 * It stores and retrieves the values and positions of every runtime variable
 ******************************************************************************/

/*******************************************************************
 * GET 
 * What it does: Search for a variable in memory
 * What it receives: The name of the variable as parameter
 * What it returns: The value of the variable stored in memory or 0
 * Note: in case variable doesn't exists it returns 0 
 *******************************************************************/
GET_RESPONSE* get(const char *name)
{
	GET_RESPONSE *response = malloc(sizeof(GET_RESPONSE));
	int i;
	for (i = 0; i < symtab_nextentry; i++)
	{
		SYMTAB current = symtab[i];
		if (strcmp(current.name, name) == 0)
		{
			response->found = true;
			response->value = memory[i];
			return response;
		}
	}
	response->found = false;
	return response;
}

/*******************************************************************
 * SAVE 
 * What it does: Inserts a key-value pair into memory
 * What it receives:
	- The name of the variable 
	- The value of the variable
 *******************************************************************/
void save(const char *name, double value)
{
	if (symtab_nextentry == MAXSTBENTRIES)
	{
		strcpy(error, "Can't add new variables, memory is full!");
		return;
	}
	int i;
	for (i = 0; i < symtab_nextentry; i++)
	{
		SYMTAB current = symtab[i];
		if (strcmp(current.name, name) == 0)
			break;
	}
	if (i == symtab_nextentry)
	{
		strcpy(symtab[i].name, name);
		symtab_nextentry++;
	}
	memory[i] = value;
}