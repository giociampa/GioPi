GioPi Pi calculation - Chudnovsky binary split algorithm (based on GMP library)

giopi   Generate arbitrary length sequences of Pi digits.
raw2txt Convert raw output to formatted text file
dat2txt Convert plain text output (eg y-cruncher) to formatted text file
compare Compare two formatted text files

Usage: Run all from a suitable command line.

giopi(.exe/.tos) digits noraw notxt noout

digits (opt)  Number of digits required (prompted for if missing)
noraw  (opt)  Skip generation of the raw format file
notxt  (opt)  Skip generation of the text format file
noout  (opt)  Combination of noraw and notxt (useful for timing runs)

raw2txt(.exe/.tos) file digits

file          Input (raw format) file
digits (opt)  Required number of output digits (defaults to digits in filename)
              (prompted for if missing - output file is "<digits>.txt")

raw2txt(.exe/.tos) inpfile outfile

inpfile (opt) Input file (defaults to "pi.dat")
outfile (opt) Output file (defaults to "pi.txt")

compare(.exe/.tos) file1 file2

file1/file2       Formatted text files
