/**@<parser.c>::**/
#include <keywords.h>
#include <parser.h>
#include <db.h>

/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman

 * Grammar definition:

 * mybc -> cmd {sep cmd }
 * sep -> ['\n'';']
 * cmd -> [ E ] end
 * E -> ['+''-'] T { ['+''-'] T } 
 * T -> F { ['*''/'] F }
 * F -> ( E ) | UINT | OCT | HEX | FLT | dbop
 * dbop -> ID = E | ID
 **************************************************/

void mybc(void)
{
	cmd();
	while (match_if_sep())
	{
		cmd();
	}
	match_end();
}

/****************************************
 * cmd -> [ E ] end
 ****************************************/
void cmd(void)
{
	/**/ double E_val; /**/
	if (is_end())
		return;
	/**/ E_val = /**/ E();
	// With the following code, the application doesn't exit in case of error
	if (strcmp(error, "") == 0)
	{
		printf("%.2lf\n", E_val);
	}
	else
	{
		//Adding \0 to string to prevent that trash-memory symbols get printed
		error[strlen(error)] = '\0';
		printf("Error! %s\n", error);
		strcpy(error, "");
	}
}

/****************************************
 * sep -> ';' | '\n'
 *
 * This method indicates if lookahead is separator
 * 
 * Return 1 in case of lookahead is sep, and matches it
 * Returns 0 in case of lookaheas isn't sep
 ****************************************/
int match_if_sep(void)
{
	switch (lookahead)
	{
	case '\n':
		match('\n');
		return 1;
	case ';':
		match(';');
		return 1;
	default:
		return 0;
	}
}

/****************************************
 * end -> QUIT | EXIT | EOF
 * 
 * This methos indicates if program 
 ****************************************/
int is_end(void)
{
	switch (lookahead)
	{
	case EOF:
		return 1;
	case EXIT:
		return 1;
	case QUIT:
		return 1;
	default:
		return 0;
	}
}

/***************************************
 * Function that matches end of program
 ***************************************/
void match_end(void)
{
	switch (lookahead)
	{
	case EXIT:
		match(EXIT);
		break;
	case QUIT:
		match(QUIT);
		break;
	default:
		match(EOF);
	}
}

/****************************************
 * E -> ['+''-'] T { ['+''-'] T } 
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

	/**/ E_val = /**/ T();
	if (signal == '-')
		E_val = -E_val;

_e_head:
	switch (lookahead)
	{
	case '+':
		match('+');
		/**/ T_val = /**/ T();
		E_val += T_val;
		goto _e_head;
		break;

	case '-':
		match('-');
		/**/ T_val = /**/ T();
		E_val -= T_val;
		goto _e_head;
		break;
	default:
		return E_val;
	}
}

/****************************************
 * T -> F { ['*''/'] F} 
 ****************************************/
double T(void)
{
	double T_val, F_val;
	/**/ T_val = /**/ F();
_t_head:
	switch (lookahead)
	{
	case '*':
		match('*');
		/**/ F_val = /**/ F();
		T_val *= F_val;
		goto _t_head;
		break;
	case '/':
		match('/');
		/**/ F_val = /**/ F();
		T_val /= F_val;
		goto _t_head;
		break;
	default:
		return T_val;
	}
	return T_val;
}

/**************************************************
 * F -> ( E ) | UINT | OCT | HEX | FLT | dbop 
 *
 * Note: OCT, UINT and Hex will be parsed to FLOAT
***************************************************/
double F(void)
{
	/**/ double F_val; /**/
	switch (lookahead)
	{
	case '(':
		match('(');
		/**/ F_val = /**/ E();
		match(')');
		break;
	//As a requirement, the application only works with float
	//So, to be able to make the parse, we need to know if its OCT or HEX
	case HEX:
		F_val = (double)strtol(lexeme, NULL, 16);
		match(HEX);
		break;
	case OCT:
		F_val = (double)strtol(lexeme, NULL, 0);
		match(OCT);
		break;
	case UINT:
	case FLOAT:
		/**/ F_val = atof(lexeme); /**/
		match(lookahead);
		break;
	default:
		F_val = dbop();
	}
	/**/ return F_val /**/;
}

/**********************************************************************
 * dbop -> ID = E | ID
 *
 * Function: DataBaseOperation
 *
 * What it does: Saves a value in memory or retrieves value from memory
 *
 * Returns:
 * 0 - In case variable not found
 * E - In case of assignment
 * ID value - In case of retrievement 
 ***********************************************************************/
double dbop()
{
	char name[MAXIDLEN + 1];
	double value = 0;
	/**/ strcpy(name, lexeme); /**/
	match(ID);

	//If next character is '=', then it needs to save value in memory
	if (lookahead == '=')
	{
		match('=');
		/**/ value = /**/ E();
		/**/ save(name, value); /**/
	}
	else
	{
		//In this case, it needs to retrieve the value from memory
		GET_RESPONSE *response = get(name);

		//Checks if variable exists in memory, if not, shows error message!
		if (!response->found)
		{
			sprintf(error, "Variable \"%s\" could not be found!", name);
		}
		else
		{
			/**/ value = /**/ response->value;
		}
	}
	return value;
}

int lookahead;
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
	}
}