OBJS = statistics
sourcefile = statistics.cc MainDict.cc TGShapedMain.cc TGShapedMain.hh

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lGui

CFLAGS = -Wall -O2 -I$(TARTSYS)/include -I$(ROOTSYS)/include -L$(TARTSYS)/lib -lanacore -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++ 

all: $(OBJS)

$(OBJS): $(sourcefile)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $(filter %.cc ,$(sourcefile))

MainDict.cc:TGShapedMain.hh wuLinkDef.h
	rootcint -f $@ -c $^


clean:
	rm -f *~ *.o $(OBJS) *Dict*

very-clean:
	rm -f $(OBJ) *.o *Dict* *~

.PHONY: clean very-clean
