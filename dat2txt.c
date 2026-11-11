#include "giopi.h"

int main(int argc, char** argv)
{
	char	srcname[256];
	char	outname[256];
	FILE	*source;
	FILE	*output;
	bool	show;
	bool	done;
	int	count;
	int	this;

	strcpy(srcname, "pi.dat");
	strcpy(outname, "pi.txt");
	
	if (argc > 1) {
		strcpy(srcname, argv[1]);
		if (argc > 2) {
			strcpy(outname, argv[2]);
		}
	}
	
	printf("Source: %s\n", srcname);
	printf("Output: %s\n\n", outname);
	
	source = fopen(srcname, "r");
	if (source == NULL) {
		printf("No source file: %s\n", srcname);
		exit(1);
	}
	output = fopen(outname, "w");
	
	show = false;
	done = false;
	count = 0;
	
	while (done == false) {
		this = getc(source);
		if ((this == EOF) || (isalpha(this) != 0)) {
			printf("Digits: %d\n", count);
			putc('\n', output);
			done = true;
		} else {
			if ((show == false) && (this == CHAR_THREE)) {
				show = true;
				putc('3', output);
				putc('.', output);
			} else if ((show == true) && (isdigit(this) != 0)) {
				putc(this, output);
				count++;
				if ((count % CHUNKCHARS) == 0) {
					if ((count % DIGITSLINE) == 0) {
						putc('\n', output);
						putc(' ', output);
					}
					putc(' ', output);
				}
			}
		}
	}
	
	fclose(source);
	fclose(output);
	
	return 0;
}
