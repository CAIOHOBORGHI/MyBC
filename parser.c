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
	/**/ float E_val; /**/
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
 * E -> ['+''-'] T { ['+''-'] T } 
 ****************************************/
float E(void)
{
	/**/
	int signal = 0;
	float E_val = 0, T_val = 0; /**/
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
float T(void)
{
	float T_val, F_val;
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
 * F -> ID | UINT | OCT | HEX | FLT | ASSGN 
*******************************************/
float F(void)
{
	/**/ float F_val; char name[MAXIDLEN + 1]; /**/
	switch (lookahead)
	{
	case '(':
		match('(');
		/**/ F_val = /**/ E();
		match(')');
		break;
	case HEX:
		F_val = (float)strtol(lexeme, NULL, 16);
		match(lookahead);
		break;
	case OCT:
		F_val = (float)strtol(lexeme, NULL, 8);
		match(lookahead);
	case UINT:
	case FLOAT:
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
	}
}