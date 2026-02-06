GioPi Pi calculation - Chudnovsky binary split algorithm

pi      Generate arbitrary length sequences of Pi digits
        (Writes formatted text file: -raw variant writes raw file)
        (*.ttp run on Atari m68040, *-s.ttp run on Atari m68000)
raw2txt Convert raw output to formatted text file
dat2txt Convert plain text output (eg y-cruncher) to formatted text file
compare Compare two formatted text files

Usage: Run all from a suitable command line.

pi      [digits] [noout]

digits  (opt) Desired digits (prompted if missing)
noout   (opt) Skip output file

raw2txt file digits

file          Input file (Output is input file with "raw" changed to "txt")
digits (opt)  Desired digits (default is digits in filename, prompts if needed)

dat2txt inpfile outfile

inpfile (opt) Input file (defaults to "pi.dat")
outfile (opt) Output file (defaults to "pi.txt")

compare file1 file2

file1/file2       Formatted text files
