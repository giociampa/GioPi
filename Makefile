BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c convert.c output.c
CMPFILES = compare.c

FLAGSALL = -O3 -fomit-frame-pointer -lm
FLAGSLOC = -ffast-math ${FLAGSALL} -lgmp -s
FLAGS000 = -m68000 -ffast-math ${FLAGSALL} -lgmp -s
FLAGS020 = -m68020 -ffast-math ${FLAGSALL} -lgmp20 -s
FLAGS040 = -m68040 -mhard-float ${FLAGSALL} -lgmp40 -s
ELFLG000 = -m68000 -ffast-math ${ELFLGALL} -lgmp
ELFLG020 = -m68020 -ffast-math ${ELFLGALL} -lgmp20
ELFLG040 = -m68040 -mhard-float ${ELFLGALL} -lgmp40
FLAGSEXE = -ffast-math ${FLAGSALL} -lgmp -s
FLAGSTST = -DTESTING

LOCALGCC = gcc
ATARIGCC = m68k-atari-mint-gcc
ATARIELF = m68k-atari-elf-gcc
ELFTOPRG = m68k-atari-elf-prg
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

atari: m68000 m68020 m68040

# --------------------------------------------------------------------------------------------------

m68000: giopi00.ttp giotst0.ttp compare0.ttp

giopi00.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

giotst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68020: giopi20.ttp giotst2.ttp compare2.ttp

giopi20.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} -o $@

giotst2.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} ${FLAGSTST} -o $@

compare2.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS020} -o $@

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp giotst4.ttp compare4.ttp

giopi40.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@

giotst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

atarielf: m68000elf m68020elf m68040elf

# --------------------------------------------------------------------------------------------------

m68000elf: giopi00.prg giotst0.prg compare0.prg

giopi00.prg: giopi00.elf
	${ELFTOPRG} giopi00.elf $@

giopi00.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG000} ${BUILDDATE} -o $@

giotst0.prg: giotst0.elf
	${ELFTOPRG} giotst0.elf $@

giotst0.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG000} ${BUILDDATE} ${ELFLGTST} -o $@

compare0.prg: compare0.elf
	${ELFTOPRG} compare0.elf $@

compare0.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${ELFLG000} -o $@

# --------------------------------------------------------------------------------------------------

m68020elf: giopi20.prg giotst2.prg compare2.prg

giopi20.prg: giopi20.elf
	${ELFTOPRG} giopi20.elf $@

giopi20.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG020} ${BUILDDATE} -o $@

giotst2.prg: giotst2.elf
	${ELFTOPRG} giotst2.elf $@

giotst2.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG020} ${BUILDDATE} ${ELFLGTST} -o $@

compare2.prg: compare2.elf
	${ELFTOPRG} compare2.elf $@

compare2.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${ELFLG020} -o $@

# --------------------------------------------------------------------------------------------------

m68040elf: giopi40.prg giotst4.prg compare4.prg

giopi40.prg: giopi40.elf
	${ELFTOPRG} giopi40.elf $@

giopi40.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG040} ${BUILDDATE} -o $@

giotst4.prg: giotst4.elf
	${ELFTOPRG} giotst4.elf $@

giotst4.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${ELFLG040} ${BUILDDATE} ${ELFLGTST} -o $@

compare4.prg: compare4.elf
	${ELFTOPRG} compare4.elf $@

compare4.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${ELFLG040} -o $@

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
	@du -b giopi giotst compare *.ttp *.prg *.exe *.dll 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

clean:
	rm -f giopi giotst compare *.ttp *.prg *.exe *.dll

veryclean: clean
	@mv README.txt README.txt.000 2>/dev/null ||:
	rm -f *.log *.run *.running *.tmp *.txt
	@mv README.txt.000 README.txt 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------
