/**@<lexer.c>::**/
// Tue Nov 17 07:55:34 PM -03 2020
/*
 * this is the start project for lexical analyser to be used
 * as the interface to the parser of the project mybc (my basic
 * calculator).
 *
 * In order to have a pattern scan we have first to implement
 * a method to ignore spaces.
 */

#include <stdio.h>
#include <ctype.h>
#include <tokens.h>
#include <constants.h>
#include <keywords.h>

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
		lexeme[i] = 0;

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
			lexeme[i] = 0;
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
			lexeme[i] = 0;
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
	int head;

	if ((head = getc(tape)) == '0')
	{

		if (isdigit(head = getc(tape)) && head <= 7)
		{

			while (isdigit(head = getc(tape)) && head <= 7)
				;

			ungetc(head, tape);

			return OCT;
		}
		else
		{

			ungetc(head, tape);

			ungetc('0', tape);

			return 0;
		}
	}

	ungetc(head, tape);

	return 0;
}
/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int head;
	int x;

	if ((head = getc(tape)) == '0')
	{

		if ((x = getc(tape)) == 'X' || x == 'x')
		{

			if (isxdigit(head = getc(tape)))
			{

				while (isxdigit(head = getc(tape)))
					;

				ungetc(head, tape);

				return HEX;
			}
			else
			{

				ungetc(head, tape);

				ungetc(x, tape);

				ungetc('0', tape);

				return 0;
			}
		}

		ungetc(x, tape);

		ungetc('0', tape);

		return 0;
	}

	ungetc(head, tape);

	return 0;
}

int gettoken(FILE *source)
{
	int token;

	skipspaces(source);

	if ((token = isID(source)))
		return token;

	if ((token = isOCT(source)))
		return token;

	if ((token = isHEX(source)))
		return token;

	if ((token = isNUM(source)))
		return token;

	token = getc(source);

	return token;
}
