#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "giopi.h"

char  runfile[NAMESIZE];

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
  char    runtext[NAMESIZE], *gotchar;
  FILE    *loghand, *runhand;
  va_list args;
  
  va_start(args, fmt);
  vsprintf(runtext, fmt, args);
  va_end(args);

  // log to console
  fprintf(stdout, "%s", runtext);
  fflush(stdout);

  // log to file
  if (logfile != NULL) {
      loghand = fopen(logfile, "ab");
      fprintf(loghand, "%s\n", runtext);
      fclose(loghand);
  }

  // log to run status
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
  remove(runfile);
}
