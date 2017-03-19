SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libananadeko.la
libananadeko_la_SOURCES = $(SUBSRC1) ananadeko_dict.cc
libananadeko_la_DEPENDENCIES = $(SUBOBJ)
libananadeko_la_LIBADD = $(SUBOBJ)
libananadeko_la_LDFLAGS = $(LDFLAGS)

ananadeko_dict.cc: $(SUBHDR) ananadeko_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
