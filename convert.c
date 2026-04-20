#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "giopi.h"

void convert(char *inpfile, char *outfile, unsigned long digits, bool quiet) {
	FILE			*inphand, *outhand;
	bool			show, done;
	unsigned long	count, this, written;

	if (!quiet) {
		printf("Converting: %s to %s\n", inpfile, outfile);
	}

	inphand = fopen(inpfile, "rb");
	if (inphand == NULL) {
		printf("ERROR: Missing input file: %s\n", inpfile);
		return;
	}
	outhand = fopen(outfile, "wb");

	show = false;
	done = false;
	count = 0;
	written = 0;

	while (done == false) {
		this = getc(inphand);
		if ((this == EOF) || (isalpha(this) != 0)) {
			if (!quiet) {
				printf("Digits: %d\n", count);
			}
			putc('\n', outhand);
			done = true;
		} else if (show == true) {
			if (isdigit(this) != 0) {
				putc(this, outhand);
				count++;
				if ((count % CHUNKCHARS) == 0) {
					if ((count % DIGITSLINE) == 0) {
						putc('\n', outhand);
						putc(' ', outhand);
					}
					putc(' ', outhand);
				}
				written++;
				if (written >= digits) {
					done = true;
				}
			}
		} else if (this == CHAR_POINT) {
			show = true;
			putc(CHAR_THREE, outhand);
			putc(CHAR_POINT, outhand);
		}
	}

	fclose(inphand);
	fclose(outhand);
}
