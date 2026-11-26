GioPi - Chudnovsky binary split algorithm Pi calculation
        Based on the gmp-chudnovsky.c code created/modified as below:
----------------------------------------------------------------------------------------------------
/* Copyright 2002, 2005 Hanhong Xue (macroxue at yahoo dot com)
 *
 * Slightly modified 2005 by Torbjorn Granlund to allow more than 2G
 *   digits to be computed.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
----------------------------------------------------------------------------------------------------

giopi   Generate arbitrary length sequences of Pi digits
giotst  (Test version of the above)
compare Compare two formatted text files.
dat2txt Convert unformatted file (e.g. y-cruncher) to formatted text file.
raw2txt Convert raw output to formatted text file.
tmp2txt Reprocess pass-9-*.tmp file (if remaining after a run crashes)

Notes:
*.exe = Windows executable
*.ttp = Atari executable (0/2/4 suffix for m68000/20/40 code)

Storage requirements (approx):
RAM:  8*digits
Disk: 6*digits (temporary files)

----------------------------------------------------------------------------------------------------

giopi [-d/--digits] digits [-1/-2/-4/-8] [--splits splits] [-n] [--noout] [-r] [--raw] [-h] [--help]

-d digits / --digits digits / digits
	Desired digits (prompted if digits not specified)
	(digit-only used in preference to others, otherwise last value specified used)

-1 / -2 / -4 / -8 / --splits splits
	Use 1/2/4/8 way version of the binary split (default = 2, any other value ignored)

-n / --noout
	Skip output file (useful for timing runs)

-r / --raw
	Generate raw output rather than text

-h / --help
	Print this help text

----------------------------------------------------------------------------------------------------

compare file1 file2 [good]

file1/2    Formatted text files
good       Display only the correct number of digits

----------------------------------------------------------------------------------------------------

dat2txt [file1] [file2]

file1      Unformatted text file (defaults to pi.dat)
file2      Formatted text file (defaults to pi.txt)

----------------------------------------------------------------------------------------------------

raw2txt file [digits]

file       Input file (output filename usually based on input)
digits     Desired digits (defaults to digits in filename, prompts if needed)

----------------------------------------------------------------------------------------------------

tmp2txt

No parameters required (all data needed is in the pass-9-*.tmp files)
