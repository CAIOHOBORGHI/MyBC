/**@<parser.c>::**/
#include <keywords.h>
#include <parser.h>
#include <db.h>

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
 * cmd -> [ E ]
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
		if (strcmp(error, "") == 0){
			printf("%.2lf\n", E_val);
		}
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

	E_val = T();
	if (signal == '-')
		E_val = -E_val;

	_e_head:
	switch(lookahead){
		case '+':
			match('+');
			T_val = T();
			E_val += T_val;
			goto _e_head;
			break;

		case '-':
			match('-');
			T_val = T();
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
	switch(lookahead){
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

/******************************************
 * F -> ( E ) | UINT | OCT | HEX | FLT | DBOP 
*******************************************/
double F(void)
{
	/**/ double F_val;/**/
	switch (lookahead)
	{
	case '(':
		match('(');
		/**/ F_val = /**/ E();
		match(')');
		break;
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
 * DBOP -> ID = E | ID
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
		if(!response->found)
		{
			sprintf(error, "Variable \"%s\" could not be found!", name);
		}else{
			/**/ value = /**/ response->value;
		}
	}
	return value;
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
	}
}