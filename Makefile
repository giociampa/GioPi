BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
RAWFILES = raw2txt.c getdigits.c logging.c output.c
CMPFILES = compare.c

FLAGS000 = -O6 -lm -lgmp -m68000 -ffast-math
FLAGS040 = -O6 -lm -lgmp -m68040 -mhard-float
FLAGSLOC = -O6 -lm -lgmp -ffast-math
FLAGSEXE = -O3 -lm -lgmp -ffast-math

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi pitst pigmp compare raw2txt raw2tst raw2gmp

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

pitst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -DTESTING -o $@

pigmp: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -DGMPOUT -o $@

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${FLAGSLOC} -o $@

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} -o $@

raw2tst: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} -DTESTING -o $@

raw2gmp: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} -DGMPOUT -o $@

# --------------------------------------------------------------------------------------------------

atari:  m68000 m68040

# --------------------------------------------------------------------------------------------------

m68000: pi0.ttp pitst0.ttp pigmp0.ttp compare0.ttp raw2txt0.ttp raw2tst0.ttp raw2gmp0.ttp

pi0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -o $@

pitst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -DTESTING -o $@

pigmp0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -DGMPOUT -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@

raw2tst0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -DTESTING -o $@

raw2gmp0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -DGMPOUT -o $@

# --------------------------------------------------------------------------------------------------

m68040: pi.ttp pitst.ttp pigmp.ttp compare.ttp raw2txt.ttp raw2tst.ttp raw2gmp.ttp

pi.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -o $@

pitst.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -DTESTING -o $@

pigmp.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -DGMPOUT -o $@

compare.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

raw2tst.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -DTESTING -o $@

raw2gmp.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -DGMPOUT -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe pitst.exe pigmp.exe compare.exe raw2txt.exe raw2tst.exe raw2gmp.exe

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

pitst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -DTESTING -o $@

pigmp.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -DGMPOUT -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} -o $@

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -o $@

raw2tst.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -DTESTING -o $@

raw2gmp.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -DGMPOUT -o $@

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross

# --------------------------------------------------------------------------------------------------

clean:
	@echo 'rm -f pi* pitst* pigmp* compare* raw2txt*  raw2gmp*' ||:
	@rm -f pi pitst pigmp compare raw2txt raw2tst raw2gmp ||:
	@rm -f pi0.ttp pitst0.ttp pigmp0.ttp compare0.ttp raw2txt0.ttp raw2tst0.ttp raw2gmp0.ttp ||:
	@rm -f pi.ttp pitst.ttp pigmp.ttp compare.ttp raw2txt.ttp raw2tst.ttp raw2gmp.ttp ||:
	@rm -f pi.exe pitst.exe pigmp.exe compare.exe raw2txt.exe raw2tst.exe raw2gmp.exe ||:

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
