BUILDDATE = -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"
TITLEBASE = -DBASENAME=\"giopi\"
TITLETEST = -DBASENAME=\"giopi-test\"

SRCFILES = giopi.c getdigits.c logging.c split.c root10005.c divide.c output.c
COMFLAGS = -O6 -lm -lgmp
FLAGS000 = -m68000 -ffast-math
FLAGS040 = -m68040 -mhard-float
FLAGSX86 = -ffast-math

LOCALGCC = gcc
ATARIGCC = m68k-atari-mintelf-gcc
CROSSGCC = x86_64-w64-mingw32-gcc

native: giopi

native-raw: giopi-r

native-all: native native-raw

testing: giopi giopi-t

giopi: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

giopi-r: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

giopi-t: ${SRCFILES}
	${LOCALGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLETEST} ${BUILDDATE} -DTESTING -o $@

m68000: giopi-s.ttp

m68000-raw: giopi-rs.ttp

m68000-all: m68000 m68000-raw

giopi-s.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -o $@

giopi-rs.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS000} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

m68040: giopi.ttp

m68040-raw: giopi-r.ttp

m68040-all: m68040 m68040-raw

giopi.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -o $@

giopi-r.ttp: ${SRCFILES}
	${ATARIGCC} ${SRCFILES} ${COMFLAGS} ${FLAGS040} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

cross: giopi.exe

cross-r: giopi-r.exe

cross-all: cross cross-r

giopi.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -o $@

giopi-r.exe: ${SRCFILES}
	${CROSSGCC} ${SRCFILES} ${COMFLAGS} ${FLAGSX86} ${TITLEBASE} ${BUILDDATE} -DRAWOUT -o $@

all: native-all m68000-all m68040-all cross-all

all-txt: native m68000 m68040 cross

all-txt: native-raw m68000-raw m68040-raw cross-r

clean: clean-native clean-m68000 clean-m68040 clean-cross

clean-all: clean-native clean-m68000 clean-m68040 clean-cross clean-objects

clean-native:
	rm -f giopi giopi-r giopi-t

clean-m68000:
	rm -f giopi-s.ttp giopi-rs.ttp

clean-m68040:
	rm -f giopi.ttp giopi-r.ttp

clean-cross:
	rm -f giopi.exe giopi-r.exe

clean-objects:
	rm -f *.o
	rm -f 1*.log 1*.raw 1*.txt
	rm -f 2*.log 2*.raw 2*.txt
	rm -f 3*.log 3*.raw 3*.txt
	rm -f 4*.log 4*.raw 4*.txt
	rm -f 0*.log 5*.raw 5*.txt
	rm -f 0*.log 6*.raw 6*.txt
	rm -f 0*.log 7*.raw 7*.txt
	rm -f 8*.log 8*.raw 8*.txt
	rm -f 9*.log 9*.raw 9*.txt
	rm -f 0*.log 0*.raw 0*.txt
