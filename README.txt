GioPi - Chudnovsky binary split algorithm Pi calculation
--------------------------------------------------------------------------------

giopi  Generate arbitrary length sequences of Pi digits
giotst (Test version of the above)
compare Compare two formatted text files.
raw2txt Convert raw output to formatted text file.
tmp2txt Reprocess pass-9-*.tmp file (if remaining after a run crashes)

Notes:
*.exe = Windows executable
*.ttp = Atari executable (0/2/4 suffix for m68000/20/40 code)

(Approximate) Storage requirements:
RAM:  8x digits
Disk: 6x digits (temporary files)

--------------------------------------------------------------------------------

giopi/tst [digits] [noout] [raw]

digits  (opt) Desired digits (prompted if missing)
noout   (opt) Skip output file (useful for timing runs)
raw     (opt) Generate raw output rather than text

--------------------------------------------------------------------------------

compare file1 file2 [good]

file1, file2  Formatted text files
good          (opt) Display only the correct number of digits

--------------------------------------------------------------------------------

raw2txt file [digits]

file          Input file (output filename usually based on input)
digits (opt)  Desired digits (defaults to digits in filename, prompts if needed)

--------------------------------------------------------------------------------

tmp2txt

No parameters required (all data needed is in the pass-9-*.tmp files)
