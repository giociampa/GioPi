BUILDDATE := -DBUILDDATE=\"$(shell date +%Y%m%d-%H%M)\"
TITLEBASE := -DBASENAME=\"giopi\"
TITLETEST := -DBASENAME=\"giopi-test\"

CFLAGSALL := -O6 -lm -lgmp
CFLAGS000 := -m68000 -ffast-math
CFLAGS040 := -m68040 -mhard-float
CFLAGSX86 := -ffast-math

NATIVEGCC := gcc
M68000GCC := m68k-atari-mintelf-gcc

native: giopi giopi-test compare dat2txt raw2txt

m68000: giopi-st.tos giopi-test-st.tos compare-st.tos dat2txt-st.tos raw2txt-st.tos

m68040: giopi.tos giopi-test.tos compare.tos dat2txt.tos raw2txt.tos

all: native m68000 m68040

giopi: giopi.c giopi.h
	${NATIVEGCC} giopi.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-test: giopi-test.c giopi.h
	${NATIVEGCC} giopi-test.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLETEST} -o $@

compare: compare.c giopi.h
	${NATIVEGCC} compare.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEBASE} -o $@

dat2txt: dat2txt.c giopi.h
	${NATIVEGCC} dat2txt.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEBASE} -o $@

raw2txt: raw2txt.c giopi.h
	${NATIVEGCC} raw2txt.c ${CFLAGSALL} ${CFLAGSX86} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-st.tos: giopi.c giopi.h
	${M68000GCC} giopi.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-test-st.tos: giopi-test.c giopi.h
	${M68000GCC} giopi-test.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLETEST} -o $@

compare-st.tos: compare.c giopi.h
	${M68000GCC} compare.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEBASE} -o $@

dat2txt-st.tos: dat2txt.c giopi.h
	${M68000GCC} dat2txt.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEBASE} -o $@

raw2txt-st.tos: raw2txt.c giopi.h
	${M68000GCC} raw2txt.c ${CFLAGSALL} ${CFLAGS000} ${BUILDDATE} ${TITLEBASE} -o $@

giopi.tos: giopi.c giopi.h
	${M68000GCC} giopi.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEBASE} -o $@

giopi-test.tos: giopi-test.c giopi.h
	${M68000GCC} giopi-test.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLETEST} -o $@

compare.tos: compare.c giopi.h
	${M68000GCC} compare.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEBASE} -o $@

dat2txt.tos: dat2txt.c giopi.h
	${M68000GCC} dat2txt.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEBASE} -o $@

raw2txt.tos: raw2txt.c giopi.h
	${M68000GCC} raw2txt.c ${CFLAGSALL} ${CFLAGS040} ${BUILDDATE} ${TITLEBASE} -o $@

clean: clean-native clean-m68000 clean-m68040

clean-native:
	rm -f giopi giopi-test compare dat2txt raw2txt

clean-m68000:
	rm -f giopi-st.tos giopi-test-st.tos compare-st.tos dat2txt-st.tos raw2txt-st.tos

clean-m68040:
	rm -f giopi.tos giopi-test.tos compare.tos dat2txt.tos raw2txt.tos
