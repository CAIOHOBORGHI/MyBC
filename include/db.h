/**@<db.h>::**/

/****************************************************************************
 * SYMTAB is an array used to store the name and position of each variable
 ****************************************************************************/
typedef struct _symtab_
{
	char name[MAXIDLEN + 1];
	int pos;
} SYMTAB;

char error[100];

float memory[MAXSTBENTRIES];

SYMTAB symtab[MAXSTBENTRIES];
int symtab_nextentry;

float get(const char *name);
void save(const char *name, float value);