OBJ = convert 
sourcefile = convert.cpp r2root.cpp r2root.hh UserDefine.hh


ROOTCFLAGS  = $(shell root-config --cflags) -I$(TARTSYS)/include -I$(ROOTSYS)/include
ROOTLIBS    = $(shell root-config --libs) -L$(TARTSYS)/lib -lanacore


CFLAGS = -Wall -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cpp ,$(sourcefile)) $(ROOTLIBS) 

clean:
	rm -f *~ *.o $(OBJ)
