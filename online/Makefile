OBJS = online
sourcefile = Online.cc

ROOTCFLAGS  = $(shell root-config --cflags) -I$(TARTSYS)/include -I$(ROOTSYS)/include
ROOTLIBS    = $(shell root-config --libs) -lGui  -L$(TARTSYS)/lib -lanacore

CFLAGS = -Wall -O2  -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all: $(OBJS)

$(OBJS): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cc ,$(sourcefile)) $(ROOTLIBS)

clean:
	rm -f *~ *.o $(OBJS) 
