SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Nadeko/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall  -I../../Core/include -I../../Nadeko/include
LDFLAGS = $(shell root-config --glibs)

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanaloop.la
libanaloop_la_SOURCES = $(SUBSRC1) analoop_dict.cc
libanaloop_la_DEPENDENCIES = $(SUBOBJ)
libanaloop_la_LIBADD = $(SUBOBJ)
libanaloop_la_LDFLAGS = $(LDFLAGS)

analoop_dict.cc: $(SUBHDR) analoop_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
