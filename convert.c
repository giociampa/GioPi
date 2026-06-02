#include "giopi.h"

// logging.c
void logthis(char *filename, char *fmt, ...);

void convert(char *inpfile, char *outfile, unsigned long digits, bool giopi, bool point) {
	FILE			*inphand, *outhand;
	bool			show, done;
	unsigned long	count, this, written, progress, percent;

	if (!giopi) {
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
  progress = 0;

	while (done == false) {
		this = getc(inphand);
		if ((this == EOF) || (isalpha(this) != 0)) {
			if (!giopi) {
				printf("Digits: %lu\n", count);
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
        if (digits > 0) {
          written++;
          if (giopi) {
            percent = (100 * written) / digits;
            if (percent > progress) {
              progress = percent;
              if (giopi) {
                logthis(NULL, "Write: Write (%ld%%)\r", percent);
              } else {
                logthis(NULL, "Write: (%ld%%)\r", percent);
              }
            }
          }
          if (written >= digits) {
            done = true;
          }
        }
			}
		} else if (point || (this == CHAR_POINT)) {
			show = true;
			putc(CHAR_THREE, outhand);
			putc(CHAR_POINT, outhand);
		}
	}

	fclose(inphand);
	fclose(outhand);
}
