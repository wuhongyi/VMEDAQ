TARGET = RIDF2DataId

ROOTCFLAGS  = $(shell $(CFG_ROOT_BIN)root-config --cflags)
ROOTLIBS    = $(shell $(CFG_ROOT_BIN)root-config --libs)
CFLAGS = -I$(TARTSYS)/include -L$(TARTSYS)/lib -lanacore

GXX = g++

all: $(TARGET)

RIDF2DataId: RIDF2DataId.C
	$(CXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTLIBS) -o $@ $^

clean:
	rm -f $(TARGET)
