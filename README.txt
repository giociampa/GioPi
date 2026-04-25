GioPi - Chudnovsky binary split algorithm Pi calculation
========================================================

pi      Generate arbitrary length sequences of Pi digits
        (*.exe = Windows executable: output is formatted .txt file)
        (*.ttp = Atari executable: output is .raw file)
raw2txt Convert raw output to formatted text file
dat2txt Convert plain text output (eg y-cruncher) to formatted text file
compare Compare two formatted text files

(For Windows: 64-bit OS required)
(For Atari: 0/000 files are 68000-compatible; 4/040 files are 68040-compatible)

Usage: Run all from a suitable command line.

pi [digits] [noout] [split]

digits  (opt) Desired digits (prompted if missing)
noout   (opt) Skip output file (useful for timing runs)
split   (opt) Stop after binary split (no output file)

raw2txt file [digits]

file          Input file (Output is input file with "raw" changed to "txt")
digits (opt)  Desired digits (default is digits in filename, prompts if needed)

dat2txt [inpfile] [outfile]

inpfile (opt) Input file (defaults to "pi.dat")
outfile (opt) Output file (defaults to "pi.txt")

compare file1 file2

file1, file2  Formatted text files
