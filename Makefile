BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
RAWFILES = raw2txt.c getdigits.c logging.c output.c
CMPFILES = compare.c
DATFILES = dat2txt.c

FLAGSALL = -O3 -fomit-frame-pointer -lm -lgmp -g
FLAGSLOC = -ffast-math ${FLAGSALL}
FLAGS000 = -m68000 -ffast-math ${FLAGSALL}
FLAGS040 = -m68040 -mhard-float ${FLAGSALL}
FLAGSEXE = -ffast-math ${FLAGSALL}

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
ATARIELF = m68k-atari-elf-gcc
PRGTOELF = m68k-atari-elf-prg
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi piraw pitst compare raw2txt dat2txt

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

piraw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -DRAWOUT -o $@

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

m68000: pi000.ttp piraw0.ttp pitst0.ttp compare0.ttp raw2txt0.ttp raw2tst0.ttp dat2txt0.ttp

pi000.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

piraw0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -o $@

pitst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DTESTING -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@

raw2tst0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -DTESTING -o $@

dat2txt0.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: pi040.ttp piraw4.ttp pitst4.ttp compare4.ttp raw2txt4.ttp raw2tst4.ttp dat2txt4.ttp

pi040.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

piraw4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -o $@

pitst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DTESTING -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

raw2tst4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -DTESTING -o $@

dat2txt4.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

atariprg: m68000elf m68040elf

# --------------------------------------------------------------------------------------------------

m68000elf: pi000.prg piraw0.prg pitst0.prg compare0.prg raw2txt0.prg raw2tst0.prg dat2txt0.prg

pi000.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${PRGTOELF) $@ $@.ttp

piraw0.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DRAWOUT -o $@
	${PRGTOELF) $@ $@.ttp

pitst0.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -DTESTING -o $@
	${PRGTOELF) $@ $@.ttp

compare0.prg: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@
	${PRGTOELF) $@ $@.ttp

raw2txt0.prg: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@
	${PRGTOELF) $@ $@.ttp

raw2tst0.prg: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -DTESTING -o $@
	${PRGTOELF) $@ $@.ttp

dat2txt0.prg: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} -o $@
	${PRGTOELF) $@ $@.ttp

# --------------------------------------------------------------------------------------------------

m68040elf: pi040.prg piraw4.prg pitst4.prg compare4.prg raw2txt4.prg raw2tst4.prg dat2txt4.prg

pi040.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

piraw4.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DRAWOUT -o $@

pitst4.prg: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -DTESTING -o $@

compare4.prg: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt4.prg: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

raw2tst4.prg: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -DTESTING -o $@

dat2txt4.prg: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe piraw.exe pitst.exe compare.exe raw2txt.exe dat2txt.exe copydlls

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

piraw.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -DRAWOUT -o $@

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
	@echo 'Copying DLL files' ||:
	@find /usr -iname "cygwin1.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross

# --------------------------------------------------------------------------------------------------

clean:
	rm -f pi piraw pitst compare raw2txt dat2txt *.prg *.ttp *.exe *.dll

veryclean: clean
	rm -f 0*.log 0*.raw 0*.txt
	rm -f 1*.log 1*.raw 1*.txt
	rm -f 2*.log 2*.raw 2*.txt
	rm -f 3*.log 3*.raw 3*.txt
	rm -f 4*.log 4*.raw 4*.txt
	rm -f 5*.log 5*.raw 5*.txt
	rm -f 6*.log 6*.raw 6*.txt
	rm -f 7*.log 7*.raw 7*.txt
	rm -f 8*.log 8*.raw 8*.txt
	rm -f 9*.log 9*.raw 9*.txt

# --------------------------------------------------------------------------------------------------

