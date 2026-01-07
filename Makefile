BUILDDATE := -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"
TITLEBASE := -DBASENAME=\"giopi\"
TITLEDISK := -DBASENAME=\"giopi-disk\"
TITLETEST := -DBASENAME=\"giopi-test\"

CFLAGSALL := -O6 -lm -lgmp
CFLAGS000 := -m68000 -ffast-math
CFLAGS040 := -m68040 -mhard-float
CFLAGSX86 := -ffast-math

NATIVEGCC := gcc
M68000GCC := m68k-atari-mintelf-gcc

native: giopi giopi-disk giopi-test compare dat2txt raw2txt

m68000: giopi-st.ttp giopi-disk-st.ttp giopi-test-st.ttp compare-st.ttp dat2txt-st.ttp raw2txt-st.ttp

m68040: giopi.ttp giopi-disk.ttp giopi-test.ttp compare.ttp dat2txt.ttp raw2txt.ttp

all: native m68000 m68040

giopi: giopi.c giopi.h
	${NATIVEGCC} giopi.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-disk: giopi-disk.c giopi.h
	${NATIVEGCC} giopi-disk.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEDISK} -o $@

giopi-test: giopi-test.c giopi.h
	${NATIVEGCC} giopi-test.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLETEST} -o $@

compare: compare.c giopi.h
	${NATIVEGCC} compare.c ${CFLAGSALL} ${CFLAGSX86} -o $@

dat2txt: dat2txt.c giopi.h
	${NATIVEGCC} dat2txt.c ${CFLAGSALL} ${CFLAGSX86} -o $@

raw2txt: raw2txt.c giopi.h
	${NATIVEGCC} raw2txt.c ${CFLAGSALL} ${CFLAGSX86} -o $@

giopi-st.ttp: giopi.c giopi.h
	${M68000GCC} giopi.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-disk-st.ttp: giopi-disk.c giopi.h
	${M68000GCC} giopi-disk.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEDISK} -o $@

giopi-test-st.ttp: giopi-test.c giopi.h
	${M68000GCC} giopi-test.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLETEST} -o $@

compare-st.ttp: compare.c giopi.h
	${M68000GCC} compare.c ${CFLAGSALL} ${CFLAGS000} -o $@

dat2txt-st.ttp: dat2txt.c giopi.h
	${M68000GCC} dat2txt.c ${CFLAGSALL} ${CFLAGS000} -o $@

raw2txt-st.ttp: raw2txt.c giopi.h
	${M68000GCC} raw2txt.c ${CFLAGSALL} ${CFLAGS000} -o $@

giopi.ttp: giopi.c giopi.h
	${M68000GCC} giopi.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-disk.ttp: giopi-disk.c giopi.h
	${M68000GCC} giopi-disk.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEDISK} -o $@

giopi-test.ttp: giopi-test.c giopi.h
	${M68000GCC} giopi-test.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLETEST} -o $@

compare.ttp: compare.c giopi.h
	${M68000GCC} compare.c ${CFLAGSALL} ${CFLAGS040} -o $@

dat2txt.ttp: dat2txt.c giopi.h
	${M68000GCC} dat2txt.c ${CFLAGSALL} ${CFLAGS040} -o $@

raw2txt.ttp: raw2txt.c giopi.h
	${M68000GCC} raw2txt.c ${CFLAGSALL} ${CFLAGS040} -o $@

clean: clean-native clean-m68000 clean-m68040
	rm -f tmp*.tmp

clean-native:
	rm -f giopi giopi-disk giopi-test compare dat2txt raw2txt

clean-m68000:
	rm -f giopi-st.ttp giopi-disk-st.ttp giopi-test-st.ttp compare-st.ttp dat2txt-st.ttp raw2txt-st.ttp

clean-m68040:
	rm -f giopi.ttp giopi-disk.ttp giopi-test.ttp compare.ttp dat2txt.ttp raw2txt.ttp
