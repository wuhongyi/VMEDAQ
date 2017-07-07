ALL: caenvmebabies

CFLAGS = -Wall -O2 -DLINUX
LOADLIBES = -lpthread -lCAENVME

libbabies.o : libbabies.h
libbbcaenvme.o : libbbcaenvme.h

MODULES = v7xx.o v1290.o v1190.o v977.o v830.o madc32.o

caenvmebabies : caenvmebabies.o libbabies.o libbbcaenvme.o bbpid.o $(MODULES)

clean :
	rm -f *.o
