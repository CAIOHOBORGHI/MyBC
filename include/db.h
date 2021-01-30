/**@<db.h>::**/
#include <stdbool.h>

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
	float value;
} GET_RESPONSE;

char error[100];

float memory[MAXSTBENTRIES];

SYMTAB symtab[MAXSTBENTRIES];
int symtab_nextentry;

GET_RESPONSE* get(const char *name);
void save(const char *name, float value);