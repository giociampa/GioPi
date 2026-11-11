TIMESTAMP = $(shell date +%Y%m%d-%H%M)
BUILDDATE = -DBUILDDATE=\"${TIMESTAMP}\"

ALLFILES = giopi giotst giopid giopidt compare raw2txt tmp2txt *.ttp *.exe *.dll *.zip
SRCFILES = giopi.c getdigits.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c
CMPFILES = compare.c
DATFILES = dat2txt.c
RAWFILES = raw2txt.c getdigits.c logging.c convert.c
TMPFILES = tmp2txt.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c

STACKTOS = -Wl,--stack,131072
STACKEXE = -Wl,--stack,16777216

FLAGSALL = -fomit-frame-pointer -ffast-math -O3 -lm
FLAGSLOC = ${FLAGSALL} -lgmp
FLAGS000 = ${FLAGSALL} -lgmp -DBUILDTYPE=\"68000\" -m68000 -msoft-float ${STACKTOS}
FLAGS020 = ${FLAGSALL} -lgmp20 -DBUILDTYPE=\"68020\" -m68020 -msoft-float ${STACKTOS}
FLAGS040 = ${FLAGSALL} -lgmp40 -DBUILDTYPE=\"68040\" -m68040 -mhard-float ${STACKTOS}
FLAGSEXE = ${FLAGSALL} -lgmp ${STACKEXE}
FLAGSTST = -DTESTING

LOCALGCC = gcc
LOCSTRIP = strip
ATARIGCC = m68k-atari-mint-gcc
M68STRIP = m68k-atari-mint-strip
CROSSGCC = x86_64-w64-mingw32-gcc
CROSTRIP = x86_64-w64-mingw32-strip

# --------------------------------------------------------------------------------------------------

local: giopi giotst compare dat2txt raw2txt tmp2txt

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

dat2txt: ${DATFILES}
	${LOCALGCC} ${DATFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

tmp2txt: ${TMPFILES}
	${LOCALGCC} ${TMPFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

copy2bin: local
	@echo "Copying files to ~/bin" ||:
	@cp compare ~/bin/0compare ||:
	@cp dat2txt ~/bin/0dat2txt ||:
	@cp raw2txt ~/bin/0raw2txt ||:
	@cp tmp2txt ~/bin/0tmp2txt ||:
	@chmod +x ~/bin/0compare ||:
	@chmod +x ~/bin/0dat2txt ||:
	@chmod +x ~/bin/0raw2txt ||:
	@chmod +x ~/bin/0tmp2txt ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

atari: local m68000 m68020 m68040

# --------------------------------------------------------------------------------------------------

m68000: giopi00.ttp giotst0.ttp compare0.ttp dat2txt0.ttp raw2txt0.ttp tmp2txt0.ttp

giopi00.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst0.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare0.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

dat2txt.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} ${BUILDDATE} -o $@
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

m68020: giopi20.ttp giotst2.ttp compare2.ttp dat2txt2.ttp raw2txt2.ttp tmp2txt2.ttp

giopi20.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst2.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS020} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare2.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

dat2txt2.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

raw2txt2.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt2.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS020} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

m68040: giopi40.ttp giotst4.ttp compare4.ttp dat2txt4.ttp raw2txt4.ttp tmp2txt4.ttp

giopi40.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

giotst4.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

compare4.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

dat2txt4.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

raw2txt4.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt4.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

cross: giopi.exe giotst.exe compare.exe dat2txt.exe raw2txt.exe tmptxt.exe copydlls

giopi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${CROSTRIP} $@
	@echo ||:

giotst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -o $@
	${CROSTRIP} $@
	@echo ||:

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${CROSTRIP} $@
	@echo ||:

dat2txt.exe: ${DATFILES}
	${CROSSGCC} ${DATFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${CROSTRIP} $@
	@echo ||:

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${CROSTRIP} $@
	@echo ||:

tmptxt.exe: ${TMPFILES}
	${CROSSGCC} ${TMPFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
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
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} -g -o $@
	@echo ||:

giopid.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} -g -o $@
	@echo ||:

giopid.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -g -o $@
	@echo ||:

giopidt: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

giopidt.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

giopidt.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

release: veryclean all
	7z a -tzip giopi-${TIMESTAMP} README.txt giopi giotst compare dat2txt raw2txt tmp2txt *.ttp *.exe *.dll
	@du -b *.zip ||:
	gh release create "${TIMESTAMP}" --notes "${TIMESTAMP} release"  "giopi-${TIMESTAMP}.zip"
	@echo ||:

# --------------------------------------------------------------------------------------------------

all: atari cross
	@touch * ||:

alldebug: atari cross debug
	@touch * ||:

# --------------------------------------------------------------------------------------------------

clean:
	@echo "Cleaning files" ||:
	@rm -f ${ALLFILES} 2>/dev/null ||:
	@echo ||:

veryclean: clean
	@mv README.txt README.txt.000 2>/dev/null ||:
	@rm -f *.log *.gol *.run *.running *.tmp *.dat *.raw *.txt ||:
	@rm -f *.LOG *.GOL *.RUN *.RUNNING *.TMP *.DAT *.RAW *.TXT ||:
	@mv README.txt.000 README.txt 2>/dev/null ||:

# --------------------------------------------------------------------------------------------------

list:
	@echo "File list" ||:
	@du -b ${ALLFILES} 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------
