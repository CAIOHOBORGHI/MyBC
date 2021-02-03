/**@<lexer.c>::**/
#include <stdio.h>
#include <ctype.h>
#include <lexer.h>

/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 *************************************************/

void skipspaces(FILE *tape)
{
	int head;

	while (isspace(head = getc(tape)))
	{
		if (head == '\n')
			break;
	}

	ungetc(head, tape);
}

/* Now we need a predicate function to recognize a string
 * begining with a letter (alpha) followed by zero or more
 * digits and letters.
 *
 * REGEX: [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) returns 1 if x \in [A-Za-z]
 *            returns 0 otherwise
 * isalnum(x) returns 1 if x \in [A-Za-z0-9]
 *            returns 0 otherwise
 */
char lexeme[MAXIDLEN + 1];
int isID(FILE *tape)
{
	int i = 0;

	if (isalpha(lexeme[i] = getc(tape)))
	{
		i++;
		while (isalnum(lexeme[i] = getc(tape)))
		{
			i++;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = '\0';

		//Checks if entry is a keyword
		int token = iskeyword(lexeme);
		if (token)
			return token;

		return ID;
	}
	ungetc(lexeme[i], tape);
	return 0;
}

/* Next, we have to implement a method to recognize decimal
 * pattern (unsigned int)
 *
 * REGEX: [1-9][0-9]* | 0
 */
int isUINT(FILE *tape)
{
	int i = 0;

	if (isdigit(lexeme[i] = getc(tape)))
	{
		if (lexeme[i] == '0')
		{
			i++;
			lexeme[i] = '\0';
			;
		}
		else
		{
			i++;
			while (isdigit(lexeme[i] = getc(tape)))
			{
				i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
		}
		return UINT;
	}

	ungetc(lexeme[i], tape);

	return 0;
}
/** Trabalho de 08/12/2020: desenvolver um analisador léxico para ponto flutuante
 *
 * REGEX:
 * ( uint '.' [0-9]*  |  '.' [0-9]+ ) ee?  |  uint ee
 * uint = [1-9][0-9]* | 0
 * ee = [eE] ['+''-']? [0-9]+
 */
int isNUM(FILE *tape)
{
	int token = 0;
	int head;
	if ((token = isUINT(tape)))
	{
		if ((head = getc(tape)) == '.')
		{
			token = FLOAT;
			while (isdigit(head = getc(tape)))
				;
			ungetc(head, tape);
		}
		else
		{
			ungetc(head, tape);
		}
	}
	else if ((head = getc(tape)) == '.')
	{
		/** decimal point has been already read **/
		if (isdigit(head = getc(tape)))
		{
			token = FLOAT;
			while (isdigit(head = getc(tape)))
				;
			ungetc(head, tape);
		}
		else
		{
			/** after decimal point another char, non-digit has been read **/
			ungetc(head, tape);
			ungetc('.', tape);
		}
	}
	else
	{
		ungetc(head, tape);
	}
	int eE;
	if (token > 0)
	{
		if (toupper(eE = getc(tape)) == 'E')
		{
			int plusminus;
			if ((plusminus = getc(tape)) == '+' || plusminus == '-')
			{
				;
			}
			else
			{
				ungetc(plusminus, tape);
				plusminus = 0;
			}
			if (isdigit(head = getc(tape)))
			{
				token = FLOAT;
				while (isdigit(head = getc(tape)))
					;
				ungetc(head, tape);
			}
			else
			{
				ungetc(head, tape);
				if (plusminus)
				{
					ungetc(plusminus, tape);
				}
				ungetc(eE, tape);
			}
		}
		else
		{
			ungetc(eE, tape);
		}
	}
	return token;
}
/* Octal pattern is defined as
 * REGEX: 0[0-7]+
 */
int isOCT(FILE *tape)
{
	int i = 0, buffer = 0;

	if ((lexeme[i] = getc(tape)) == '0')
	{
		i++;
		if (isdigit(lexeme[i] = getc(tape)))
		{
			//Converting char to digit
			buffer = lexeme[i] - '0';
		}

		if (buffer <= 7)
		{
			i++;
			while (isdigit(lexeme[i] = getc(tape)))
			{
				buffer = lexeme[i] - '0';
				if (buffer <= 7)
					i++;
			}

			ungetc(lexeme[i], tape);
			lexeme[i] = '\0';
			return OCT;
		}
		else
		{
			while (i > 0)
			{
				ungetc(lexeme[i], tape);
				i--;
			}
			return 0;
		}
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = '\0';
	return 0;
}
/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int i = 0;
	if ((lexeme[i] = getc(tape)) == '0')
	{
		i++;
		if ((lexeme[i] = getc(tape)) == 'X' || lexeme[i] == 'x')
		{
			i++;
			if (isxdigit(lexeme[i] = getc(tape)))
			{
				i++;
				while (isxdigit(lexeme[i] = getc(tape)))
					i++;

				ungetc(lexeme[i], tape);
				lexeme[i] = '\0';
				return HEX;
			}
			else
			{
				while (i > 0)
					ungetc(lexeme[i], tape);

				lexeme[0] = '\0';
				return 0;
			}
		}
		while (i >= 0)
		{
			ungetc(lexeme[i], tape);
			i--;
		}

		lexeme[0] = '\0';
		return 0;
	}

	ungetc(lexeme[0], tape);
	lexeme[0] = '\0';
	return 0;
}

int gettoken(FILE *source)
{
	int token;

	skipspaces(source);

	if ((token = isID(source)))
		return token;

	if ((token = isHEX(source)))
		return token;

	if ((token = isOCT(source)))
		return token;

	if ((token = isNUM(source)))
		return token;

	token = getc(source);

	return token;
}
