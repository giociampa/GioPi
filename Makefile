BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c convert.c output.c
RAWFILES = raw2txt.c getdigits.c logging.c convert.c output.c
DATFILES = dat2txt.c logging.c convert.c
CMPFILES = compare.c

FLAGSALL = -O3 -fomit-frame-pointer -lm -lgmp -s
FLAGSLOC = -ffast-math ${FLAGSALL}
FLAGS000 = -m68000 -ffast-math ${FLAGSALL}
FLAGS040 = -m68040 -mhard-float ${FLAGSALL}
FLAGSEXE = -ffast-math ${FLAGSALL}

FLAGSRAW = -DRAWOUT
FLAGSTST = -DTESTING

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi piraw pitst compare raw2txt dat2txt

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

piraw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSRAW} -o $@

pitst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -o $@

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${FLAGSLOC} -o $@

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} -o $@

dat2txt: ${DATFILES}
	${LOCALGCC} ${DATFILES} ${FLAGSLOC} -o $@

# --------------------------------------------------------------------------------------------------

atari: m68000 m68040

# --------------------------------------------------------------------------------------------------

m68000: pi000.ttp piraw0.ttp pitst0.ttp compare0.ttp raw2txt0.ttp dat2txt0.ttp

pi000.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

piraw0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSRAW} -o $@

pitst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@

dat2txt0.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: pi040.ttp piraw4.ttp pitst4.ttp compare4.ttp raw2txt4.ttp dat2txt4.ttp

pi040.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

piraw4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSRAW} -o $@

pitst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

dat2txt4.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe piraw.exe pitst.exe compare.exe raw2txt.exe dat2txt.exe copydlls

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

piraw.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSRAW} -o $@

pitst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} -o $@

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -o $@

dat2txt.exe: ${DATFILES}
	${CROSSGCC} ${DATFILES} ${FLAGSEXE} -o $@

# --------------------------------------------------------------------------------------------------

copydlls:
	@echo 'Copying DLL files' ||:
	@find /usr -iname "cygwin1.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

list:
	@ls -l pi piraw pitst compare raw2txt dat2txt *.ttp *.exe *.dll 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross list

# --------------------------------------------------------------------------------------------------

clean:
	rm -f pi piraw pitst compare raw2txt dat2txt *.ttp *.exe *.dll

veryclean: clean
	@mv README.txt README.txt.000 ||:
	rm -f *.log *.raw *.txt
	@mv README.txt.000 README.txt ||:

# --------------------------------------------------------------------------------------------------
