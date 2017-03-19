SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../../Core/include
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -I../../Core/include
LDFLAGS = $(shell root-config --glibs) 

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanabrips.la
libanabrips_la_SOURCES = $(SUBSRC1) anabrips_dict.cc
libanabrips_la_DEPENDENCIES = $(SUBOBJ)
libanabrips_la_LIBADD = $(SUBOBJ)
libanabrips_la_LDFLAGS = $(LDFLAGS)

anabrips_dict.cc: $(SUBHDR) anabrips_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
