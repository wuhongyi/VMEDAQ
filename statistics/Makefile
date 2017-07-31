OBJS = statistics
sourcefile = statistics.cc 

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lGui

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -I$(ROOTSYS)/include -L$(TARTSYS)/lib -lanacore -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all: $(OBJS)

$(OBJS): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))


clean:
	rm -f *~ *.o $(OBJS) 

very-clean:
	rm -f $(OBJ) *.o *~

.PHONY: clean very-clean
