BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"

SRCFILES = giopi.c getdigits.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c
CMPFILES = compare.c
RAWFILES = rawtxt.c getdigits.c logging.c convert.c
TMPFILES = tmp2txt.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c

FLAGSALL = -fomit-frame-pointer -ffast-math -O3 -lm
FLAGSLOC = ${FLAGSALL} -lgmp
FLAGS000 = -m68000 -msoft-float ${FLAGSALL} -lgmp
FLAGS020 = -m68020 -msoft-float ${FLAGSALL} -lgmp20
FLAGS040 = -m68040 -mhard-float ${FLAGSALL} -lgmp40
FLAGSEXE = ${FLAGSALL} -lgmp
FLAGSTST = -DTESTING

LOCALGCC = gcc
LOCSTRIP = strip
ATARIGCC = m68k-atari-mint-gcc
M68STRIP = m68k-atari-mint-strip
ATARIELF = m68k-atari-elf-gcc
ELFTOPRG = m68k-atari-elf-prg
CROSSGCC = x86_64-w64-mingw32-gcc
CROSTRIP = x86_64-w64-mingw32-strip

# --------------------------------------------------------------------------------------------------

local: giopi giotst compare rawtxt tmptxt

giopi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

giotst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -o $@
	${LOCSTRIP} $@
	@echo ||:

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

rawtxt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

tmptxt: ${TMPFILES}
	${LOCALGCC} ${TMPFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

atari: local m68000 m68020 m68040

# --------------------------------------------------------------------------------------------------

m68000: giopi00.ttp giotst0.ttp compare0.ttp rawtxt0.ttp tmptxt0.ttp

giopi00.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} -o $@
	${M68STRIP} $@
	@echo ||:

rawtxt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@
	${M68STRIP} $@
	@echo ||:

tmptxt0.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

m68020: giopi20.ttp giotst2.ttp compare2.ttp rawtxt2.ttp tmptxt2.ttp

giopi20.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst2.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare2.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS020} -o $@
	${M68STRIP} $@
	@echo ||:

rawtxt2.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS020} -o $@
	${M68STRIP} $@
	@echo ||:

tmptxt2.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp giotst4.ttp compare4.ttp rawtxt4.ttp tmptxt4.ttp

giopi40.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} -o $@
	${M68STRIP} $@
	@echo ||:

rawtxt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@
	${M68STRIP} $@
	@echo ||:

tmptxt4.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

atarielf: local m68000elf m68020elf m68040elf

# --------------------------------------------------------------------------------------------------

m68000elf: giopi00e.ttp giotst0e.ttp compar0e.ttp rawtxt0e.ttp tmptxt0e.ttp

giopi00e.ttp: giopi00e.elf
	${ELFTOPRG} giopi00e.elf $@
	chmod +x $@
	@echo ||:

giopi00e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

giotst0e.ttp: giotst0e.elf
	${ELFTOPRG} giotst0e.elf $@
	chmod +x $@
	@echo ||:

giotst0e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${ELFLGTST} -o $@

compar0e.ttp: compare0e.elf
	${ELFTOPRG} compare0e.elf $@
	chmod +x $@
	@echo ||:

compare0e.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${FLAGS000} -o $@

rawtxt0e.ttp: rawtxt0e.elf
	${ELFTOPRG} rawtxt0e.elf $@
	chmod +x $@
	@echo ||:

rawtxt0e.elf: ${RAWFILES}
	${ATARIELF} ${RAWFILES} ${FLAGS000} -o $@

tmptxt0e.ttp: tmptxt0e.elf
	${ELFTOPRG} tmptxt0e.elf $@
	chmod +x $@
	@echo ||:

tmptxt0e.elf: ${TMPFILES}
	${ATARIELF} ${TMPFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68020elf: giopi20e.ttp giotst2e.ttp compar2e.ttp rawtxt2e.ttp tmptxt2e.ttp

giopi20e.ttp: giopi20e.elf
	${ELFTOPRG} giopi20e.elf $@
	chmod +x $@
	@echo ||:

giopi20e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

giotst2e.ttp: giotst2e.elf
	${ELFTOPRG} giotst2e.elf $@
	chmod +x $@
	@echo ||:

giotst2e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${ELFLGTST} -o $@

compar2e.ttp: compare2e.elf
	${ELFTOPRG} compare2e.elf $@
	chmod +x $@
	@echo ||:

compare2e.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${FLAGS000} -o $@

rawtxt2e.ttp: rawtxt0e.elf
	${ELFTOPRG} rawtxt0e.elf $@
	chmod +x $@
	@echo ||:

rawtxt2e.elf: ${RAWFILES}
	${ATARIELF} ${RAWFILES} ${FLAGS000} -o $@

tmptxt2e.ttp: tmptxt0e.elf
	${ELFTOPRG} tmptxt0e.elf $@
	chmod +x $@
	@echo ||:

tmptxt2e.elf: ${TMPFILES}
	${ATARIELF} ${TMPFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040elf: giopi40e.ttp giotst4e.ttp compar4e.ttp rawtxt4e.ttp tmptxt4e.ttp

giopi40e.ttp: giopi40e.elf
	${ELFTOPRG} giopi40e.elf $@
	chmod +x $@
	@echo ||:

giopi40e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@

giotst4e.ttp: giotst4e.elf
	${ELFTOPRG} giotst4e.elf $@
	chmod +x $@
	@echo ||:

giotst4e.elf: ${SRCFILES}
	${ATARIELF} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${ELFLGTST} -o $@

compar4e.ttp: compare4e.elf
	${ELFTOPRG} compare4e.elf $@
	chmod +x $@
	@echo ||:

compare4e.elf: ${CMPFILES}
	${ATARIELF} ${CMPFILES} ${FLAGS000} -o $@

rawtxt4e.ttp: rawtxt0e.elf
	${ELFTOPRG} rawtxt0e.elf $@
	chmod +x $@
	@echo ||:

rawtxt4e.elf: ${RAWFILES}
	${ATARIELF} ${RAWFILES} ${FLAGS000} -o $@

tmptxt4e.ttp: tmptxt0e.elf
	${ELFTOPRG} tmptxt0e.elf $@
	chmod +x $@
	@echo ||:

tmptxt4e.elf: ${TMPFILES}
	${ATARIELF} ${TMPFILES} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

cross: local giopi.exe giotst.exe compare.exe rawtxt.exe tmptxt.exe copydlls

giopi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${CROSTRIP} $@
	@echo ||:

giotst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -o $@
	${CROSTRIP} $@
	@echo ||:

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} -o $@
	${CROSTRIP} $@
	@echo ||:

rawtxt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -o $@
	${CROSTRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

copydlls:
	@echo 'Copying DLL files' ||:
	@find /usr -iname "cygwin1.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

list:
	@echo 'File list' ||:
	@du -b giopi giotst compare rawtxt *.elf *.ttp *.exe *.dll 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

clean:
	rm -f giopi giotst compare rawtxt tmptxt *.elf *.ttp *.exe *.dll
	@echo ||:

veryclean: clean
	@echo 'Cleaning files' ||:
	@mv README.txt README.txt.000 2>/dev/null ||:
	@rm -f *.log *.gol *.run *.running *.tmp *.raw *.txt
	@rm -f *.LOG *.GOL *.RUN *.RUNNING *.TMP *.RAW *.TXT
	@mv README.txt.000 README.txt 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

all: atari cross list

# --------------------------------------------------------------------------------------------------

allelf: atarielf cross list

# --------------------------------------------------------------------------------------------------
