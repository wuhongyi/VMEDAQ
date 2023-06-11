OBJ = pedo 
sourcefile = main.cc pedo.cc pedo.hh UserDefine.hh


ROOTCFLAGS  = $(shell root-config --cflags) -I$(TARTSYS)/include -I$(ROOTSYS)/include
ROOTLIBS    = $(shell root-config --libs)


CFLAGS = -Wall -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cc ,$(sourcefile)) $(ROOTLIBS) 

clean:
	rm -f *~ *.o $(OBJ) save.* *.sh *.root
