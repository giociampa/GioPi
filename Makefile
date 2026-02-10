BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
RAWFILES = raw2txt.c getdigits.c logging.c output.c
CMPFILES = compare.c
DATFILES = dat2txt.c

FLAGSALL = -O3 -lm -lgmp
FLAGSLOC = ${FLAGSALL} -ffast-math
FLAGS000 = ${FLAGSALL} -m68000 -ffast-math
FLAGS040 = ${FLAGSALL} -m68040 -mhard-float
FLAGSEXE = ${FLAGSALL} -ffast-math

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi pitst compare raw2txt copydlls

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

pitst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -DTESTING -o $@

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${FLAGSLOC} -o $@

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} -o $@

dat2txt: ${DATFILES}
	${LOCALGCC} ${DATFILES} ${FLAGSLOC} -o $@

# --------------------------------------------------------------------------------------------------

atari: m68000 m68040

# --------------------------------------------------------------------------------------------------

m68000: pi000.ttp pitst0.ttp compare0.ttp raw2txt0.ttp

pi000.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -o $@

pitst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DTESTING -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@

dat2txt0.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: pi040.ttp pitst4.ttp compare4.ttp raw2txt4.ttp

pi040.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -o $@

pitst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DTESTING -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

dat2txt4.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe pitst.exe compare.exe raw2txt.exe copydlls

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

pitst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -DTESTING -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} -o $@

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -o $@

dat2txt.exe: ${DATFILES}
	${CROSSGCC} ${DATFILES} ${FLAGSEXE} -o $@

# --------------------------------------------------------------------------------------------------

copydlls:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross

# --------------------------------------------------------------------------------------------------

clean:
	@echo 'rm -f pi* pitst* compare* raw2txt* *.dll' ||:
	@rm -f pi pitst compare raw2txt ||:
	@rm -f pi000.ttp pitst0.ttp compare0.ttp raw2txt0.ttp ||:
	@rm -f pi040.ttp pitst4.ttp compare4.ttp raw2txt4.ttp ||:
	@rm -f pi.exe pitst.exe compare.exe raw2txt.exe ||:
	@rm -f *.dll ||:

veryclean: clean
	@echo 'rm -f *.log *.raw *.txt' ||:
	@rm -f 0*.log 0*.raw 0*.txt ||:
	@rm -f 1*.log 1*.raw 1*.txt ||:
	@rm -f 2*.log 2*.raw 2*.txt ||:
	@rm -f 3*.log 3*.raw 3*.txt ||:
	@rm -f 4*.log 4*.raw 4*.txt ||:
	@rm -f 5*.log 5*.raw 5*.txt ||:
	@rm -f 6*.log 6*.raw 6*.txt ||:
	@rm -f 7*.log 7*.raw 7*.txt ||:
	@rm -f 8*.log 8*.raw 8*.txt ||:
	@rm -f 9*.log 9*.raw 9*.txt ||:

# --------------------------------------------------------------------------------------------------

