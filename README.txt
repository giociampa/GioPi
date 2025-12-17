GioPi Pi calculation - Chudnovsky binary split algorithm

giopi   Generate arbitrary length sequences of Pi digits.
raw2txt Convert raw output to formatted text file
dat2txt Convert plain text output (eg y-cruncher) to formatted text file
compare Compare two formatted text files

Usage: Run all from a suitable command line.

giopi(.exe/.tos) digits noraw notxt noout

digits (opt)  Desired digits (prompted if missing)
noraw  (opt)  Skip generation of the raw format file
notxt  (opt)  Skip generation of the text format file
noout  (opt)  Combination of noraw and notxt (useful for timing runs)

raw2txt(.exe/.tos) file digits

file          Input file (Output is input file with "raw" changed to "txt")
digits (opt)  Desired digits (default: digits in filename, prompted if needed)

dat2txt(.exe/.tos) inpfile outfile

inpfile (opt) Input file (defaults to "pi.dat")
outfile (opt) Output file (defaults to "pi.txt")

compare(.exe/.tos) file1 file2

file1/file2       Formatted text files
