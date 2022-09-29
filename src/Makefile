CC = g++
LIBS = -L../lib -L. -L/usr/local/lib -lgpibusb -lftd2xx `root-config --libs`
LIBS2 = -L../lib -L. -L/usr/local/lib -lgpibusb -lftd2xx
OPTIMIZE = -O2
CFLAGS = -Wall -I/usr/local/include -I../include `root-config --cflags`


.SUFFIXES : .o .cc

.cc.o : 
	${CC} ${CFLAGS} -c $<

ex1 : libgpibusb.a ex1.o
	${CC} -o $@ ex1.o ${LIBS} ${OPTIMIZE}

ex2 : libgpibusb.a ex2.o
	${CC} -o $@ ex2.o ${LIBS} ${OPTIMIZE}

ex3 : libgpibusb.a ex3.o
	${CC} -o $@ ex3.o ${LIBS} ${OPTIMIZE}

gettemp : libgpibusb.a gettemp.o
	${CC} -o $@ gettemp.o ${LIBS} ${OPTIMIZE}

pulsegen :  pulsegen.o
	${CC} -o $@ pulsegen.o ${LIBS2} ${OPTIMIZE}

pulsegen-win :  pulsegen-win.o
	${CC} -o $@ pulsegen-win.o ${LIBS2} ${OPTIMIZE}

hvtest : libgpibusb.a hvtest.o
	${CC} -o $@ hvtest.o ${LIBS} ${OPTIMIZE}

DACtest : libgpibusb.a DACtest.o
	${CC} -o $@ DACtest.o ${LIBS} ${OPTIMIZE}

TEMPtest : libgpibusb.a TEMPtest.o
	${CC} -o $@ TEMPtest.o ${LIBS} ${OPTIMIZE}

solarpaneltest : libgpibusb.a solarpaneltest.o
	${CC} -o $@ solarpaneltest.o ${LIBS} ${OPTIMIZE}

dmm1 : libgpibusb.a dmm1.o
	${CC} -o $@ dmm1.o ${LIBS} ${OPTIMIZE}

anlgain : libgpibusb.a anlgain.o
	${CC} -o $@ anlgain.o ${LIBS} ${OPTIMIZE}

twodevices : twodevices.o
	${CC} -o $@ twodevices.o ${LIBS} ${OPTIMIZE}

afg3102 : afg3102.o
	${CC} -o $@ afg3102.o ${LIBS} ${OPTIMIZE}

oscillo : oscillo.o
	${CC} -o $@ oscillo.o ${LIBS} ${OPTIMIZE}

getwaves : getwaves.o
	${CC} -o $@ getwaves.o ${LIBS2} ${OPTIMIZE}

waves2root : waves2root.o
	${CC} -o $@ waves2root.o ${LIBS} ${OPTIMIZE}

getwave1 : getwave1.o
	${CC} -o $@ getwave1.o ${LIBS} ${OPTIMIZE}

getwave2 : getwave2.o
	${CC} -o $@ getwave2.o ${LIBS} ${OPTIMIZE}

interact : interact.o
	${CC} -o $@ interact.o ${LIBS2} -lreadline ${OPTIMIZE}

testwfmpre : testwfmpre.o
	${CC} -o $@ testwfmpre.o ${LIBS} ${OPTIMIZE}

hvtest : hvtest.o
	${CC} -o $@ hvtest.o ${LIBS} ${OPTIMIZE}

hvtest12 : hvtest12.o
	${CC} -o $@ hvtest12.o ${LIBS} ${OPTIMIZE}

ccgpib : ccgpib.o
	${CC} -o $@ ccgpib.o ${LIBS} ${OPTIMIZE}

libgpibusb.a: 
	cd ../lib; make

libgpibusb.so: 
	cd ../lib; make


clean: 
	/bin/rm -f core *.o *~
