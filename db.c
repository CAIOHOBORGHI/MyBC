/**@<db.c>::**/
#include <string.h>
#include <constants.h> 
#include <db.h>

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
float get(const char *name)
{
	int i;
	for (i = 0; i < symtab_nextentry; i++)
	{
		SYMTAB current = symtab[i];
		if (strcmp(current.name, name) == 0)
		{
			return memory[i];
		}
	}
	return 0;
}

/*******************************************************************
 * SAVE 
 * What it does: Inserts a key-value pair into memory
 * What it receives:
	- The name of the variable 
	- The value of the variable
 *******************************************************************/
void save(const char *name, float value)
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