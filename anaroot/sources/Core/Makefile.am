
SUBSRC0 := $(wildcard src/*.cc)
SUBHDR0 := $(wildcard include/*.hh)

if USE_MINOS
SUBSRC := $(SUBSRC0)
SUBHDR := $(SUBHDR0)
else
SUBSRC := $(shell l=($(SUBSRC0)); echo $${l[@]//*minos*})
SUBHDR := $(shell l=($(SUBHDR0)); echo $${l[@]//*minos*})
endif

SUBOBJ := $(SUBSRC:.cc=.o)

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 $(MINOS_FEM_CFLAGS)
LDFLAGS = $(shell root-config --glibs) $(MINOS_FEM_LIBS)

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanacore.la
libanacore_la_SOURCES = $(anacore_acq_sources) $(SUBSRC) anacore_dict.cc
libanacore_la_DEPENDENCIES = $(SUBOBJ)
libanacore_la_LIBADD = $(SUBOBJ)
libanacore_la_LDFLAGS = $(LDFLAGS)

if USE_MINOS
ROOTCINTFLAGS = $(DEFAULT_INCLUDES) $(INCLUDES) -DUSE_MINOS
else
ROOTCINTFLAGS = $(DEFAULT_INCLUDES) $(INCLUDES)
endif

anacore_dict.cc: $(SUBHDR) anacore_linkdef.hh 
	rootcint -f $@ -c -p $(ROOTCINTFLAGS) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
