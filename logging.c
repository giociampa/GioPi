#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "giopi.h"

char  runfile[NAMESIZE];
char  runtext[NAMESIZE];

void loginit(char *logfile) {
  FILE  *loghand, *runhand;
  char  *dotlog;

  loghand = fopen(logfile, "wb");
  fclose(loghand);
  
  dotlog = strstr(logfile, ".log");
  if (dotlog == NULL) {
    strcpy(runfile, logfile);
  } else {
    strncpy(runfile, logfile, dotlog - logfile);
    runfile[dotlog - logfile] = 0;
  }
  strcat(runfile, ".run");

  runhand = fopen(runfile, "wb");
  fclose(runhand);
}

void logthis(char *logfile, char *fmt, ...) {
  FILE    *loghand, *runhand;
  char    *gotchar;
  va_list args;
  
  va_start(args, fmt);
  vsprintf(runtext, fmt, args);
  va_end(args);

  fprintf(stdout, "%s", runtext);
  fflush(stdout);

  if (logfile != NULL) {
      loghand = fopen(logfile, "ab");
      fprintf(loghand, "%s", runtext);
      fclose(loghand);
  }

  gotchar = strstr(runtext, "\r");
  if (gotchar != NULL) {
    gotchar[0] = 0;
  }

  gotchar = strstr(runtext, "\n");
  if (gotchar != NULL) {
    gotchar[0] = 0;
  }

  runhand = fopen(runfile, "wb");
  fprintf(runhand, "%s", runtext);
  fclose(runhand);

}

void logdone() {
  unsigned long pass;
  char          filename[NAMESIZE];
  
  remove(runfile);

  for (pass = 0 ; pass < 3 ; pass++) {
    sprintf(filename, "pass-%lu-p.tmp", pass);
    remove(filename);

    sprintf(filename, "pass-%lu-q.tmp", pass);
    remove(filename);

    sprintf(filename, "pass-%lu-t.tmp", pass);
    remove(filename);
  }
}
