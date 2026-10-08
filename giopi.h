#ifndef _GIOPI_H
#define _GIOPI_H 1

#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <gmp.h>

#define A   545140134
#define B   13591409
#define C   640320
#define C24 711822400
#define D24 15367680

#define BITS_PER_DIGIT  3.32192809488736234787
#define DIGITS_PER_ITER 14.1816474627254776555
#define DOUBLE_PREC     53
#define LEEWAY          16

#define CHUNKCOUNT  5
#define CHUNKCHARS  10
#define DIGITSLINE  50
#define WHOLELINE   (DIGITSLINE + 6)
#define NAMESIZE    256
#define WRITECHUNK  1000000
#define DEBUG_FILE  "ZZZDEBUG.TXT"
#define STACKSIZE   131072L

#define CHAR_THREE  '3'
#define CHAR_POINT  '.'

#ifndef BUILDDATE
#define BUILDDATE   __TIMESTAMP__
#endif

#ifndef BUILDTYPE
#if defined __linux__
#define BUILDTYPE   "Linux"
#elif defined __atarist__
#define BUILDTYPE   "Atari"
#elif defined __WIN64__
#define BUILDTYPE   "Windows"
#else
#define BUILDTYPE   "Unknown"
#endif
#endif

#ifdef TESTING
#define BUILDNAME   "GioTst"
#else
#define BUILDNAME   "GioPi"
#endif

#endif
