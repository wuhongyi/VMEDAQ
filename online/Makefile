OBJ = online
sourcefile = Online.C 

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs)

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -I$(ROOTSYS)/include -L$(TARTSYS)/lib -lanacore -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.C ,$(sourcefile))

clean:
	rm -f *~ *.o $(OBJ)
