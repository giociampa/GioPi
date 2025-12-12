#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "giopi.h"

int main(int argc, char** argv)
{
	char	inpfile[256], outfile[256];
	FILE	*inphand, *outhand;
	bool	show, done;
	int	count, this;

	strcpy(inpfile, "pi.dat");
	strcpy(outfile, "pi.txt");
	
	if (argc > 1) {
		strcpy(inpfile, argv[1]);
		if (argc > 2) {
			strcpy(outfile, argv[2]);
		}
	}
	
	printf("Converting: %s to %s\n", inpfile, outfile);

	inphand = fopen(inpfile, "r");
	if (inphand == NULL) {
		printf("ERROR: Missing input file: %s\n", inpfile);
		return EXIT_FAILURE;
	}
	outhand = fopen(outfile, "w");
	
	show = false;
	done = false;
	count = 0;
	
	while (done == false) {
		this = getc(inphand);
		if ((this == EOF) || (isalpha(this) != 0)) {
			printf("Digits: %d\n", count);
			putc('\n', outhand);
			done = true;
		} else {
			if ((show == false) && (this == CHAR_THREE)) {
				show = true;
				putc('3', outhand);
				putc('.', outhand);
			} else if ((show == true) && (isdigit(this) != 0)) {
				putc(this, outhand);
				count++;
				if ((count % CHUNKCHARS) == 0) {
					if ((count % DIGITSLINE) == 0) {
						putc('\n', outhand);
						putc(' ', outhand);
					}
					putc(' ', outhand);
				}
			}
		}
	}
	
	fclose(inphand);
	fclose(outhand);
	
	return EXIT_SUCCESS;
}
