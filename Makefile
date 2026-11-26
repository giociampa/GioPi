TIMESTAMP = $(shell date +%Y%m%d-%H%M)
BUILDDATE = -DBUILDDATE=\"${TIMESTAMP}\"

ALLFILES = giopi giotst giopid giopidt chudpi testpi compare dat2txt raw2txt tmp2txt *.ttp *.exe *.dll *.zip
SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c convert.c tmptxt.c output.c
CMPFILES = compare.c
DATFILES = dat2txt.c convert.c logging.c 
RAWFILES = raw2txt.c getdigits.c logging.c convert.c
TMPFILES = tmp2txt.c tmptxt.c logging.c split.c root10005.c divide.c convert.c output.c
GMPFILES = chudpi.c getdigits.c logging.c output.c convert.c

STACKTOS = -Wl,--stack,131072
STACKEXE = -Wl,--stack,16777216

FLAGSALL = -fomit-frame-pointer -ffast-math -O3 -lm
FLAGSLOC = ${FLAGSALL} -lgmp
FLAGS000 = ${FLAGSALL} -lgmp -DBUILDTYPE=\"68000\" -m68000 -msoft-float ${STACKTOS}
FLAGS040 = ${FLAGSALL} -lgmp40 -DBUILDTYPE=\"68040\" -m68040 -mhard-float ${STACKTOS}
FLAGSEXE = ${FLAGSALL} -lgmp ${STACKEXE}
FLAGSTST = -DTESTING

LOCALGCC = gcc
LOCSTRIP = strip
ATARIGCC = m68k-atari-mint-gcc
M68STRIP = m68k-atari-mint-strip
WIN64GCC = x86_64-w64-mingw32-gcc
W64STRIP = x86_64-w64-mingw32-strip

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

# --------------------------------------------------------------------------------------------------

gmp-all: gmp-local gmp-m68040

gmp-local: chudpi testpi

gmp-m68040: chudpi.ttp testpi.ttp

chudpi: ${GMPFILES}
	${LOCALGCC} ${GMPFILES} ${FLAGSLOC} ${BUILDDATE} -o $@
	${LOCSTRIP} $@
	@echo ||:

testpi: ${GMPFILES}
	${LOCALGCC} ${GMPFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -o $@
	${LOCSTRIP} $@
	@echo ||:

chudpi.ttp: ${GMPFILES}
	${ATARIGCC} ${GMPFILES} ${FLAGS040} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

testpi.ttp: ${GMPFILES}
	${ATARIGCC} ${GMPFILES} ${FLAGS040} ${BUILDDATE} ${FLAGSTST} -o $@
	${M68STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

copybin: local
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

copydlls:
	@echo "Copying DLL files" ||:
	@find /usr -iname "*msys-2*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@find /usr -iname "*gmp*.dll" -exec cp "{}" . \; 2>/dev/null ||:
	@echo ||:

# --------------------------------------------------------------------------------------------------

atari: m68000 m68040

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

dat2txt0.ttp: ${DATFILES}
	${ATARIGCC} ${DATFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

raw2txt0.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${FLAGS000} ${BUILDDATE} -o $@
	${M68STRIP} $@
	@echo ||:

tmp2txt0.ttp: ${TMPFILES}
	${ATARIGCC} ${TMPFILES} ${FLAGS000} ${BUILDDATE} -o $@
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

win64: giopi.exe giotst.exe compare.exe dat2txt.exe raw2txt.exe tmptxt.exe copydlls

giopi.exe: ${SRCFILES}
	${WIN64GCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${W64STRIP} $@
	@echo ||:

giotst.exe: ${SRCFILES}
	${WIN64GCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -o $@
	${W64STRIP} $@
	@echo ||:

compare.exe: ${CMPFILES}
	${WIN64GCC} ${CMPFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${W64STRIP} $@
	@echo ||:

dat2txt.exe: ${DATFILES}
	${WIN64GCC} ${DATFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${W64STRIP} $@
	@echo ||:

raw2txt.exe: ${RAWFILES}
	${WIN64GCC} ${RAWFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${W64STRIP} $@
	@echo ||:

tmptxt.exe: ${TMPFILES}
	${WIN64GCC} ${TMPFILES} ${FLAGSEXE} ${BUILDDATE} -o $@
	${W64STRIP} $@
	@echo ||:

# --------------------------------------------------------------------------------------------------

all: local atari win64
	@touch * ||:

# --------------------------------------------------------------------------------------------------

release: veryclean all zip-linux zip-atari zip-win64
	@du -b *.zip ||:
	gh release create "${TIMESTAMP}" --notes "${TIMESTAMP} release" giopi-${TIMESTAMP}-linux.zip giopi-${TIMESTAMP}-atari.zip giopi-${TIMESTAMP}-win64.zip
	@echo ||:

zip-linux:
	@echo Creating giopi-${TIMESTAMP}-linux.zip ||:
	@7z a -tzip giopi-${TIMESTAMP}-linux README.txt giopi giotst compare dat2txt raw2txt tmp2txt 1>/dev/null 2>/dev/null ||:
	@echo ||:

zip-atari:
	@echo Creating giopi-${TIMESTAMP}-atari.zip ||:
	@7z a -tzip giopi-${TIMESTAMP}-atari README.txt *.ttp 1>/dev/null 2>/dev/null ||:
	@echo ||:

zip-win64:
	@echo Creating giopi-${TIMESTAMP}-win64.zip ||:
	@7z a -tzip giopi-${TIMESTAMP}-win64 README.txt *.exe *.dll 1>/dev/null 2>/dev/null ||:
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
	${WIN64GCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} -g -o $@
	@echo ||:

giopidt: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${FLAGSLOC} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

giopidt.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${FLAGS000} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

giopidt.exe: ${SRCFILES}
	${WIN64GCC} ${SRCFILES} ${FLAGSEXE} ${BUILDDATE} ${FLAGSTST} -g -o $@
	@echo ||:

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
