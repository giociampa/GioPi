GioPi - Pi calculation (for PC, Pi, ST, etc.)

Based around the GMP library (and some example code for it).

giopi   - generate arbitrary length sequences of Pi digits.
raw2txt - convert raw output to text (the m68k code fails on occasion so need a separate processor).

Usage: (Run from a suitable command line)

giopi(.exe/.tos) digits noraw notxt noout noout

digits : Number of digits required (if no valid number specified it will be prompted for)
noraw  : (Optional) Skip generation of the raw format file
notxt  : (Optional) Skip generation of the text format file
noout  : (Optional) Combination of noraw and notxt - useful for timing runs

raw2txt file digits

file   : Input (raw format) file
digits : (Optional) Required number of output digits (will use digits in filename by default)
