#define A   545140134
#define B   13591409
#define C   640320
#define C24 711822400
#define D24 15367680

#define BITS_PER_DIGIT    3.32192809488736234787
#define DIGITS_PER_ITER   14.1816474627254776555
#define DOUBLE_PREC       53
#define LEEWAY            32
#define TEXTSIZE          64
#define TEMPFILEFORMAT    "tmp_%02lu_%s.tmp"

#define P1 (pstack[splitdepth])
#define Q1 (qstack[splitdepth])
#define T1 (tstack[splitdepth])
#define P2 (pstack[splitdepth+1])
#define Q2 (qstack[splitdepth+1])
#define T2 (tstack[splitdepth+1])

#define CHUNKCOUNT 5
#define CHUNKCHARS 10
#define DIGITSLINE 50
#define WHOLELINE (DIGITSLINE + 6)

#define CHAR_POINT 46
#define CHAR_THREE 51

#ifndef BASENAME
#define BASENAME __BASE_FILE__
#endif

#ifndef BUILDDATE
#define BUILDDATE __TIMESTAMP__
#endif
