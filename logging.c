#include "giopi.h"

char  runfile[NAMESIZE];

void loginit(char *logfile) {
  FILE  *loghand, *runhand;
  char  *dotlog;

  loghand = fopen(logfile, "wb");
  fclose(loghand);
  remove(logfile);
  
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
  remove(runfile);
}

void logthis(char *logfile, char *fmt, ...) {
  char    runtext[NAMESIZE], *gotchar;
  FILE    *loghand, *runhand;
  va_list args;
  bool    debugging = false;

  if (logfile != NULL) {
    debugging = (strcasecmp(logfile, DEBUG_FILE) == 0);
  }
  
  va_start(args, fmt);
  vsprintf(runtext, fmt, args);
  va_end(args);

  // log to console?
  if (!debugging) {
    fprintf(stdout, "%s", runtext);
    fflush(stdout);
  }

  // log to file?
  if (logfile != NULL) {
      loghand = fopen(logfile, "ab");
      fprintf(loghand, "%s", runtext);
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
