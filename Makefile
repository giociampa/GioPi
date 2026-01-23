BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"
TITLEBASE = -DBASENAME=\"pi\"
TITLETEST = -DBASENAME=\"pi-test\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
CMPFILES = compare.c
RAWFILES = raw2txt.c

COMFLAGS = -O6 -lm -lgmp
FLAGS000 = -m68000 -ffast-math
FLAGS040 = -m68040 -mhard-float
FLAGSX86 = -ffast-math

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi pi-raw pi-tst compare raw2txt

m68000: pi-s.ttp pi-raw-s.ttp pi-tst-s.ttp compare-s.ttp raw2txt-s.ttp 

m68040: pi.ttp pi-raw.ttp pi-tst.ttp compare.ttp raw2txt.ttp

cross: pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe

all: local m68000 m68040 cross

# --------------------------------------------------------------------------------------------------

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi-s.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-s.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-s.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

# --------------------------------------------------------------------------------------------------

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${COMFLAGS} ${FLAGSX86} -o $@

compare-s.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${COMFLAGS} ${FLAGS000} -o $@

compare.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${COMFLAGS} ${FLAGS040} -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${COMFLAGS} ${FLAGSX86} -o $@

# --------------------------------------------------------------------------------------------------

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${COMFLAGS} ${FLAGSX86} -o $@

raw2txt-s.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${COMFLAGS} ${FLAGS000} -o $@

raw2txt.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${COMFLAGS} ${FLAGS040} -o $@

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${COMFLAGS} ${FLAGSX86} -o $@

# --------------------------------------------------------------------------------------------------

clean: clean-local clean-atari clean-cross clean-results

clean-local:
	rm -f pi pi-raw pi-tst compare raw2txt

clean-atari: clean-m68000 clean-m68040

clean-cross:
	rm -f pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe

clean-m68000:
	rm -f pi-s.ttp pi-raw-s.ttp pi-tst-s.ttp compare.ttp raw2txt.ttp

clean-m68040:
	rm -f pi.ttp pi-raw.ttp pi-tst.ttp compare.ttp raw2txt.ttp

clean-results:
	@rm -f *.o ||:
	@rm -f 1*.log 1*.raw 1*.txt ||:
	@rm -f 2*.log 2*.raw 2*.txt ||:
	@rm -f 3*.log 3*.raw 3*.txt ||:
	@rm -f 4*.log 4*.raw 4*.txt ||:
	@rm -f 0*.log 5*.raw 5*.txt ||:
	@rm -f 0*.log 6*.raw 6*.txt ||:
	@rm -f 0*.log 7*.raw 7*.txt ||:
	@rm -f 8*.log 8*.raw 8*.txt ||:
	@rm -f 9*.log 9*.raw 9*.txt ||:
	@rm -f 0*.log 0*.raw 0*.txt ||:
	@echo Results cleaned ||:
