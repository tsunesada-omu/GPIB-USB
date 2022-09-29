CC = g++
#LIBSRCS = MyDevice.cc PrologixGpibUsb.cc HttpDevice.cc Wfmpre.cc DictWfmpre.cc
LIBSRCS = MyDevice.cc PrologixGpibUsb.cc HttpDevice.cc Wfmpre.cc
LIBOBJS = ${LIBSRCS:.cc=.o}
LIBS = -L. -L/usr/local/lib -lftd2xx 
OPTIMIZE = -O2
CFLAGS = -Wall -fPIC -I../include -I/usr/local/include


.SUFFIXES : .o .cc

.cc.o : 
	${CC} ${CFLAGS} -c $<


all: libgpibusb.a libgpibusb.so

#DictWfmpre.cc: ../include/dVLinkDefWfmpre.h ../include/Wfmpre.h
#	${ROOTSYS}/bin/rootcint -f $@ -c ${CFLAGS} ../include/Wfmpre.h ../include/dVLinkDefWfmpre.h

libgpibusb.a: ${LIBOBJS}
	/bin/rm -f $@
	ar vr $@ ${LIBOBJS}
	ranlib $@

libgpibusb.so: ${LIBOBJS}
	${CC} ${LIBS} -shared $^ -o $@; /bin/cp -f libgpibusb.so ../src

clean: 
	/bin/rm -f core *.o *~ Dict*.cc
