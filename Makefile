BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"
TITLEBASE = -DBASENAME=\"giopi\"
TITLETEST = -DBASENAME=\"giopi-test\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
CMPFILES = compare.c
RAWFILES = raw2txt.c

COMFLAGS = -O6 -lm -lgmp
FLAGS000 = -m68000 -ffast-math
FLAGS040 = -m68040 -mhard-float
FLAGSX86 = -ffast-math

MINTCC04 = ${HOME}/Cross/gcc/atari/mint/bin/m68k-atari-mint-gcc
MINTCC13 = m68k-atari-mintelf-gcc
MINTCC15 = ${HOME}/Cross/gcc/atari/mintelf/bin/m68k-atari-mintelf-gcc

LOCALGCC = gcc
ATARIGCC = ${MINTCC13}
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi pi-raw pi-tst compare raw2txt

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

# --------------------------------------------------------------------------------------------------

st04: pi-s-04.ttp pi-raw-s-04.ttp pi-tst-s-04.ttp compare-s.ttp raw2txt-s.ttp

tt04: pi-04.ttp pi-raw-04.ttp pi-tst-04.ttp compare.ttp raw2txt.ttp

pi-s-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-s-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-s-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-04.ttp: ${SRCFILES}
	${MINTCC04} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

# --------------------------------------------------------------------------------------------------

st13: pi-s-13.ttp pi-raw-s-13.ttp pi-tst-s-13.ttp compare-s.ttp raw2txt-s.ttp

tt13: pi-13.ttp pi-raw-13.ttp pi-tst-13.ttp compare.ttp raw2txt.ttp

pi-s-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-s-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-s-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-13.ttp: ${SRCFILES}
	${MINTCC13} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

# --------------------------------------------------------------------------------------------------

st15: pi-s-15.ttp pi-raw-s-15.ttp pi-tst-s-15.ttp compare-s.ttp raw2txt-s.ttp

tt15: pi-15.ttp pi-raw-15.ttp pi-tst-15.ttp compare.ttp raw2txt.ttp

pi-s-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-s-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-s-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

pi-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-15.ttp: ${SRCFILES}
	${MINTCC15} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe

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

# todo: fix cross before adding here
all: local st04 tt04 st13 tt13 st15 tt15

# --------------------------------------------------------------------------------------------------

clean: clean-local clean-atari clean-cross clean-rawesults

clean-local:
	rm -f pi pi-raw pi-tst compare raw2txt

clean-atari: clean-st clean-tt clean-mint000 clean-mint040

clean-cross:
	rm -f pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe

clean-st:
	rm -f pi-s*.ttp pi-raw-s*.ttp pi-tst-s*.ttp compare*.ttp raw2txt*.ttp

clean-tt:
	rm -f pi*.ttp pi-raw*.ttp pi-tst*.ttp compare*.ttp raw2txt*.ttp

clean-mint000:
	rm -f pi-s-m*.ttp pi-raw-s-m*.ttp pi-tst-s-m*.ttp compare-s*.ttp raw2txt-s*.ttp

clean-mint040:
	rm -f pi-m*.ttp pi-raw-m*.ttp pi-tst-m*.ttp compare-m*.ttp raw2txt-m*.ttp

clean-rawesults:
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
