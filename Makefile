TIMESTAMP = $(shell date +%Y%m%d-%H%M)
BUILDDATE = -DBUILDDATE=\"${TIMESTAMP}\"

ALLFILES = giopi giotst giopid giopidt compare raw2txt tmp2txt *.ttp *.exe *.dll *.zip
SRCFILES = giopi.c getdigits.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c
CMPFILES = compare.c
RAWFILES = raw2txt.c getdigits.c logging.c convert.c
TMPFILES = tmp2txt.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c

FLAGSALL = -fomit-frame-pointer -ffast-math -O3 -lm
FLAGSLOC = -DBUILDTYPE=\"Linux\" ${FLAGSALL} -lgmp
FLAGS000 = -DBUILDTYPE=\"m68000\" -m68000 -msoft-float ${FLAGSALL} -lgmp
FLAGS020 = -DBUILDTYPE=\"m68020\" -m68020 -msoft-float ${FLAGSALL} -lgmp20
FLAGS040 = -DBUILDTYPE=\"m68040\" -m68040 -mhard-float ${FLAGSALL} -lgmp40
FLAGSEXE = -DBUILDTYPE=\"Windows\" ${FLAGSALL} -lgmp
FLAGSTST = -DTESTING

LOCALGCC = gcc
LOCSTRIP = strip
ATARIGCC = m68k-atari-mint-gcc
M68STRIP = m68k-atari-mint-strip
CROSSGCC = x86_64-w64-mingw32-gcc
CROSTRIP = x86_64-w64-mingw32-strip

# --------------------------------------------------------------------------------------------------

local: giopi giotst compare raw2txt tmp2txt

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
	@cp $@ ~/bin/0$@ ||:
	@echo ||:

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@cp $@ ~/bin/0$@ ||:
	@echo ||:

tmp2txt: ${TMPFILES}
	${LOCALGCC} ${TMPFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@cp $@ ~/bin/0$@ ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

atari: local m68000 m68020 m68040

# --------------------------------------------------------------------------------------------------

m68000: giopi00.ttp giotst0.ttp compare0.ttp raw2txt0.ttp tmp2txt0.ttp

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

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt0.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

m68020: giopi20.ttp giotst2.ttp compare2.ttp raw2txt2.ttp tmp2txt2.ttp

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

raw2txt2.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS020} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt2.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp giotst4.ttp compare4.ttp raw2txt4.ttp tmp2txt4.ttp

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

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt4.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

cross: local giopi.exe giotst.exe compare.exe raw2txt.exe tmptxt.exe copydlls

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

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} -o $@
	${CROSTRIP} $@
	@echo ||:

tmptxt.exe: ${TMPFILES}
	${CROSSGCC} ${TMPFILES} ${FLAGSEXE} -o $@
	${CROSTRIP} $@
	@echo ||:

copydlls:
	@echo "Copying DLL files" ||:
	@find /usr -iname "cygwin1.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

debug: giopid giopid.ttp giopid.exe giopidt giopidt.ttp giopidt.exe copydlls

giopid: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -g3 -o $@
	@echo ||:

giopid.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -g3 -o $@
	@echo ||:

giopid.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -g3 -o $@
	@echo ||:

giopidt: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -g3 -o $@
	@echo ||:

giopidt.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -g3 -o $@
	@echo ||:

giopidt.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -g3 -o $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

release: veryclean all
	7z a -tzip giopi-${TIMESTAMP} README.txt giopi giotst compare raw2txt tmp2txt *.ttp *.exe *.dll
	@du -b *.zip ||:
	gh release create "${TIMESTAMP}" --notes "${TIMESTAMP} release"  "giopi-${TIMESTAMP}.zip"
	@echo ||:

# --------------------------------------------------------------------------------------------------

all: atari cross list
	@touch * ||:

alldebug: atari cross debug list
	@touch * ||:

# --------------------------------------------------------------------------------------------------

clean:
	@echo "Cleaning files" ||:
	@rm -f ${ALLFILES} 2>/dev/null ||:
	@echo ||:

veryclean: clean
	@mv README.txt README.txt.000 2>/dev/null ||:
	@rm -f *.log *.gol *.run *.running *.tmp *.raw *.txt ||:
	@rm -f *.LOG *.GOL *.RUN *.RUNNING *.TMP *.RAW *.TXT ||:
	@mv README.txt.000 README.txt 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

list:
	@echo "File list" ||:
	@du -b ${ALLFILES} 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------
