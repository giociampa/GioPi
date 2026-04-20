#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "giopi.h"

// convert.c
void convert(char *inpfile, char *outfile, unsigned long digits, bool quiet);

int main(int argc, char** argv)
{
	char	inpfile[NAMESIZE], outfile[NAMESIZE];
	strcpy(inpfile, "pi.dat");
	strcpy(outfile, "pi.txt");
	
	if (argc > 1) {
		strcpy(inpfile, argv[1]);
		if (argc > 2) {
			strcpy(outfile, argv[2]);
		}
	}

	convert(inpfile, outfile, 0, false);
	
	return EXIT_SUCCESS;
}
