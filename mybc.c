/**@<mybc.c>::**/
/*************************************************
 * Project My Basic Calculator - Group 6

 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <parser.h>

void mybc(void);


FILE *source;
extern int lookahead;

int main(int argc, char const *argv[])
{
	source = fopen(argv[1], "r");

	switch (argc)
	{
	case 1:
		source = stdin;
		break;
	default:
		if (source == NULL)
		{
			fprintf(stderr,
					"argv[1]: cannot open... exiting with error status\n",
					argv[1]);
			exit(-1);
		}
	}

	lookahead = gettoken(source);

	mybc();

	exit(0);
}
