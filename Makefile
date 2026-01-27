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

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

# --------------------------------------------------------------------------------------------------

local: pi pi-raw pi-tst compare raw2txt

pi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

compare: ${CMPFILES}
	${LOCALGCC} ${CMPFILES} ${COMFLAGS} ${FLAGSX86} -o $@

raw2txt: ${RAWFILES}
	${LOCALGCC} ${RAWFILES} ${COMFLAGS} ${FLAGSX86} -o $@

# --------------------------------------------------------------------------------------------------

m68000: pi-st.ttp pi-raw-st.ttp pi-tst-st.ttp compare-st.ttp raw2txt-st.ttp

pi-st.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw-st.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst-st.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

compare-st.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${COMFLAGS} ${FLAGS000} -o $@

raw2txt-st.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${COMFLAGS} ${FLAGS000} -o $@

# --------------------------------------------------------------------------------------------------

m68040: pi.ttp pi-raw.ttp pi-tst.ttp compare.ttp raw2txt.ttp

pi.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

compare.ttp: ${CMPFILES}
	${ATARIGCC} ${CMPFILES} ${COMFLAGS} ${FLAGS040} -o $@

raw2txt.ttp: ${RAWFILES}
	${ATARIGCC} ${RAWFILES} ${COMFLAGS} ${FLAGS040} -o $@

# --------------------------------------------------------------------------------------------------

cross: pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe

pi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

pi-raw.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

pi-tst.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

compare.exe: ${CMPFILES}
	${CROSSGCC} ${CMPFILES} ${COMFLAGS} ${FLAGSX86} -o $@

raw2txt.exe: ${RAWFILES}
	${CROSSGCC} ${RAWFILES} ${COMFLAGS} ${FLAGSX86} -o $@

# --------------------------------------------------------------------------------------------------

all: local m68000 m68040 cross

# --------------------------------------------------------------------------------------------------

clean:
	@echo 'rm -f pi* pi-raw* pi-tst* compare* raw2txt*' ||:
	@rm -f pi pi-raw pi-tst compare raw2txt ||:
	@rm -f pi.exe pi-raw.exe pi-tst.exe compare.exe raw2txt.exe ||:
	@rm -f pi.ttp pi-raw.ttp pi-tst.ttp compare.ttp raw2txt.ttp ||:
	@rm -f pi-st.ttp pi-raw-st.ttp pi-tst-st.ttp compare-st.ttp raw2txt-st.ttp ||:
	@echo 'rm -f *.log *.raw *.txt' ||:
	@rm -f 0*.log 0*.raw 0*.txt ||:
	@rm -f 1*.log 1*.raw 1*.txt ||:
	@rm -f 2*.log 2*.raw 2*.txt ||:
	@rm -f 3*.log 3*.raw 3*.txt ||:
	@rm -f 4*.log 4*.raw 4*.txt ||:
	@rm -f 5*.log 5*.raw 5*.txt ||:
	@rm -f 6*.log 6*.raw 6*.txt ||:
	@rm -f 7*.log 7*.raw 7*.txt ||:
	@rm -f 8*.log 8*.raw 8*.txt ||:
	@rm -f 9*.log 9*.raw 9*.txt ||:

# --------------------------------------------------------------------------------------------------
