SUBSRC1 = $(wildcard src/*.cc)
SUBOBJ = $(SUBSRC1:.cc=.o)
SUBHDR = $(wildcard include/*.hh) 

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall  -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
LDFLAGS = $(shell root-config --glibs)

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanaloopexample.la
libanaloopexample_la_SOURCES = $(SUBSRC1) analoopexample_dict.cc
libanaloopexample_la_DEPENDENCIES = $(SUBOBJ)
libanaloopexample_la_LIBADD = $(SUBOBJ)
libanaloopexample_la_LDFLAGS = $(LDFLAGS)

analoopexample_dict.cc: $(SUBHDR) analoopexample_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
