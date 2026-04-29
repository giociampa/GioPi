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

local: giopi gioraw giotst compare raw2txt dat2txt

giopi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

gioraw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSRAW} -o $@

giotst: ${SRCFILES}
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

m68000: giopi00.ttp gioraw0.ttp giotst0.ttp compare0.ttp raw2txt0.ttp dat2txt0.ttp

giopi00.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

gioraw0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSRAW} -o $@

giotst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@

dat2txt0.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp gioraw4.ttp giotst4.ttp compare4.ttp raw2txt4.ttp dat2txt4.ttp

giopi40.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

gioraw4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSRAW} -o $@

giotst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@

dat2txt4.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: giopi.exe gioraw.exe giotst.exe compare.exe raw2txt.exe dat2txt.exe copydlls

giopi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

gioraw.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSRAW} -o $@

giotst.exe: ${SRCFILES}
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
	@ls -l giopi gioraw giotst compare raw2txt dat2txt *.ttp *.exe *.dll 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross list

# --------------------------------------------------------------------------------------------------

clean:
	rm -f giopi gioraw giotst compare raw2txt dat2txt *.ttp *.exe *.dll

veryclean: clean
	@mv README.txt README.txt.000 ||:
	rm -f *.log *.raw *.run *.tmp *.txt
	@mv README.txt.000 README.txt ||:

# --------------------------------------------------------------------------------------------------
