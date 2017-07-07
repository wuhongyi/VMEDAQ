ALL : cmdvme

CFLAGS = -Wall -O2 -DLINUX 
LOADLIBES = -lCAENVME

cmdvme.o : libbbcaenvme.h
libbbcaenvme.o : libbbcaenvme.h

cmdvme : cmdvme.o libbbcaenvme.o

clean : 
	rm -f *.o
