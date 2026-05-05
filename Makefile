BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c convert.c output.c
CMPFILES = compare.c

FLAGSALL = -O3 -fomit-frame-pointer -lm -lgmp -s
FLAGSLOC = -ffast-math ${FLAGSALL}
FLAGS000 = -m68000 -ffast-math ${FLAGSALL}
FLAGS040 = -m68040 -mhard-float ${FLAGSALL}
FLAGSEXE = -ffast-math ${FLAGSALL}
FLAGSTST = -DTESTING

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: giopi giotst compare

giopi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@

giotst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -o $@

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${FLAGSLOC} -o $@

# --------------------------------------------------------------------------------------------------

atari: m68000 m68040

# --------------------------------------------------------------------------------------------------

m68000: giopi00.ttp giotst0.ttp compare0.ttp

giopi00.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

giotst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp giotst4.ttp compare4.ttp

giopi40.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

giotst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: giopi.exe giotst.exe compare.exe copydlls

giopi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@

giotst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} -o $@

# --------------------------------------------------------------------------------------------------

copydlls:
	@echo 'Copying DLL files' ||:
	@find /usr -iname "cygwin1.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

list:
	@ls -l giopi giotst compare *.ttp *.exe *.dll 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross list

# --------------------------------------------------------------------------------------------------

clean:
	rm -f giopi giotst compare *.ttp *.exe *.dll

veryclean: clean
	@mv README.txt README.txt.000 ||:
	rm -f *.log *.run *.tmp *.txt
	@mv README.txt.000 README.txt ||:

# --------------------------------------------------------------------------------------------------
