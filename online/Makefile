OBJ = online
OBJS = online2
sourcefile = main.cc MainFrame.hh MainFrame.cc OnlineDict.cc
sourcefile2 = Online.cc

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lGui

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -I$(ROOTSYS)/include -L$(TARTSYS)/lib -lanacore -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all:$(OBJ) $(OBJS)

$(OBJ): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

$(OBJS): $(sourcefile2)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile2))


OnlineDict.cc:  MainFrame.hh Linkdef.h
	rootcint -f $@ -c $^

clean:
	rm -f *~ *.o $(OBJ) $(OBJS) *Dict*
