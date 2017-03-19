SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Reconstruction/BigRIPS/include 
CXXFLAGS = -Wall -g -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include -I../../Reconstruction/BigRIPS/include 
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanasamurai.la
libanasamurai_la_SOURCES = $(SUBSRC1) anasamurai_dict.cc
libanasamurai_la_DEPENDENCIES = $(SUBOBJ)
libanasamurai_la_LIBADD = $(SUBOBJ)
libanasamurai_la_LDFLAGS = $(LDFLAGS)

anasamurai_dict.cc: $(SUBHDR) anasamurai_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
