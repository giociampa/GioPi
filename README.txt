GioPi - Chudnovsky binary split algorithm Pi calculation
========================================================

giopi  Generate arbitrary length sequences of Pi digits / giotst (Test version of the above)
compare Compare two formatted text files.
raw2txt Convert raw output to formatted text file.
tmp2txt Reprocess pass-9-*.tmp file (if remaining after a run crashes)

Note:
*.exe = Windows executable
*.ttp = Atari executable (0/2/4 suffix for m68000/20/40 code)

Usage: Run all from a suitable command line.

----------------------------------------------------------------------------------------------------

giopi/tst [digits] [noout] [raw]

digits  (opt) Desired digits (prompted if missing)
noout   (opt) Skip output file (useful for timing runs)
raw     (opt) Generate raw output rather than text

----------------------------------------------------------------------------------------------------

compare file1 file2 [good]

file1, file2  Formatted text files
good          (opt) Display only the correct number of digits

----------------------------------------------------------------------------------------------------

raw2txt file [digits]

file          Input file (Output is input file with "raw" changed to "txt")
digits (opt)  Desired digits (default is digits in filename, prompts if needed)

----------------------------------------------------------------------------------------------------

tmp2txt

No parameters required (all data needed is in the pass-9-*.tmp files)
