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

Storage requirements (approx):
RAM:  8*digits
Disk: 6*digits (temporary files)

--------------------------------------------------------------------------------

giopi [-d digits] [-1] [-2] [-4] [-8] [-n] [-r] [-h] [places]

-d digits  Desired digits (prompted if digits not specified)
places     Alternative to -d parameter (-d has priority if both used)
-1/2/4/8   Use 1/2/4/8 way version of the binary split (default = 2)
-n         Skip output file (useful for timing runs)
-r         Generate raw output rather than text
-h         Print this help text

--------------------------------------------------------------------------------

compare file1 file2 [good]

file1/2    Formatted text files
good       Display only the correct number of digits

--------------------------------------------------------------------------------

raw2txt file [digits]

file       Input file (output filename usually based on input)
digits     Desired digits (defaults to digits in filename, prompts if needed)

--------------------------------------------------------------------------------

tmp2txt

No parameters required (all data needed is in the pass-9-*.tmp files)
