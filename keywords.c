/**@<keywords.c>::**/
#include <string.h>
#include <keywords.h>

char *keyword[] = {
    "quit",
    "exit",
};

int iskeyword(const char *name)
{
    int i;
    for (i = QUIT; i < EXIT + 1; i++)
    {
        if (strcmp(name, keyword[i - QUIT]) == 0)
            return i;
    }
    return 0;
}