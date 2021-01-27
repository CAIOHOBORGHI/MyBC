/**@<parser.c>::**/
#include <keywords.h>
#include <parser.h>

/* mybc -> cmd  { sepcm cmd } end 
* cmd -> E | <> 
* end -> QUIT | EXIT | EOF 
* sepocm = '\n' | ';' 
*/

char error[100];

void mybc(void)
{
	cmd();
	while (!is_end())
	{
		match(lookahead);
		cmd();
	}
}

/****************************************
 * cmd -> E | <>
 ****************************************/
void cmd(void)
{
	/**/ double E_val; /**/
	switch (lookahead)
	{
	case '\n':
	case ';':
	case QUIT:
	case EXIT:
	case EOF:
		break;
	default:
		/**/ E_val = /**/ E();
		if (strcmp(error, "") == 0)
			printf("%lg\n", E_val);
		else
		{
			error[strlen(error)] = '\0';
			printf("Error! %s\n", error);
			strcpy(error, "");
		}
	}
}

/****************************************
 * end -> QUIT | EXIT | EOF
 * 
 * Function that indicates if program ended
 ****************************************/
int is_end(void)
{
	switch (lookahead)
	{
	case EOF:
		match(lookahead);
		return 1;
	case EXIT:
		match(EXIT);
		return 1;
	case QUIT:
		match(QUIT);
		return 1;
	default:
		return 0;
	}
}

/****************************************
 * E -> ['+''-'] T { (+) T } 
 ****************************************/
double E(void)
{
	/**/
	int signal = 0;
	double E_val = 0, T_val = 0; /**/
	if (lookahead == '+' || lookahead == '-')
	{
		/**/ signal = lookahead; /**/
		match(lookahead);
	}

	T_val = T();
	/**/ if (signal == '-')
	{
		T_val = -T_val;
	} /**/

	/**/ E_val = /**/ T_val;
	while (lookahead == '+' || lookahead == '-')
	{
		/*(1a)*/ int oplus = lookahead; /**/
		match(lookahead);
		T_val = T();
		/*(1)*/
		if (oplus == '+')
			E_val += T_val;
		else
			E_val -= T_val;
		/**/
	}
	return E_val;
}

/* T -> F { (2a) (*) F (2) } */
double T(void)
{
	double T_val, F_val;
	/**/ T_val = /**/ F();
	while (lookahead == '*' || lookahead == '/')
	{
		/*(2a)*/ int otimes = lookahead; /**/
		match(lookahead);
		/**/ F_val = /**/ F();
		/*(2)*/
		if (otimes == '*')
			T_val *= F_val;
		else
			T_val /= F_val;
		/**/
	}
	return T_val;
}

/*  F ->  ( E )
 *      | (3) n
 *      | (4) v  [ = E ] */
double F(void)
{
	/**/ double F_val;
	char name[MAXIDLEN + 1]; /**/
	switch (lookahead)
	{
	case '(':
		match('(');
		/**/ F_val = /**/ E();
		match(')');
		break;
	case UINT:
	case FLOAT:
		// case HEX:
		// case OCT:
		/**/ F_val = atof(lexeme); /**/
		match(lookahead);
		break;
	default:
		/**/ strcpy(name, lexeme); /**/
		match(ID);

		//If next character is ASSIGN, then it needs to save value in memory
		if (lookahead == '=')
		{
			match('=');
			/**/ F_val = /**/ E();
			/**/ save(name, F_val); /**/
		}
		else
		{
			//In this case, it needs to retrieve the value from memory
			/**/ F_val = get(name); /**/
		}
	}
	/**/ return F_val /**/;
}

void match(int expected)
{
	if (lookahead == expected)
	{
		lookahead = gettoken(source);
	}
	else
	{
		error[0] = '\0';
		sprintf(error, "token mismatch: expected %d whereas found %d", expected, lookahead);
		// exit(-2);
	}
}

//Symbol Table Entries
double memory[MAXSTBENTRIES];
typedef struct _symtab_
{
	char name[MAXIDLEN + 1];
	int pos;
} SYMTAB;

SYMTAB symtab[MAXSTBENTRIES];
int symtab_nextentry = 0;

/*******************************************************************
 * Function GET 
 * What it does: Search for a variable in memory
 * What it receives: The name of the variable as parameter
 * What it returns: The value of the variable stored in memory or 0
 * Note: in case variable doesn't exists it returns 0 
 *******************************************************************/
double get(const char *name)
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

void save(const char *name, double value)
{
	if (symtab_nextentry == MAXSTBENTRIES)
	{
		strcmp(error, "Can't add new variables, memory is full!");
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