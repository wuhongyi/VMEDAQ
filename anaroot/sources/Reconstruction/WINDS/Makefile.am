SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include -I../../Reconstruction/BigRIPS/include
CXXFLAGS = -Wall -g -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include -I../../Reconstruction/BigRIPS/include
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanawinds.la
libanawinds_la_SOURCES = $(SUBSRC1) anawinds_dict.cc
libanawinds_la_DEPENDENCIES = $(SUBOBJ)
libanawinds_la_LIBADD = $(SUBOBJ)
libanawinds_la_LDFLAGS = $(LDFLAGS)

anawinds_dict.cc: $(SUBHDR) anawinds_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
