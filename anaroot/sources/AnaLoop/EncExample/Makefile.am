
SUBSRC0 = $(wildcard src/*.cc)
SUBHDR0 = $(wildcard include/*.hh) 

if USE_MINOS
SUBSRC := $(SUBSRC0)
SUBHDR := $(SUBHDR0)
else
SUBSRC := $(shell l=($(SUBSRC0)); echo $${l[@]//*MINOS*})
SUBHDR := $(shell l=($(SUBHDR0)); echo $${l[@]//*MINOS*})
endif

SUBOBJ = $(SUBSRC:.cc=.o)

AUTOMAKE_OPTIONS = foreign

INCLUDES = -Iinclude -I$(ROOTSYS)/include -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
CXXFLAGS = -fPIC -Iinclude $(shell root-config --cflags) -g -Wall  -I../Core/include -I../../Core/include -I../../Nadeko/include $(patsubst %,-I%,$(wildcard ../../Reconstruction/*/include))
LDFLAGS = $(shell root-config --glibs)

include_HEADERS = $(SUBHDR) $(SUBHDRF)

lib_LTLIBRARIES = libanaloopencexample.la
libanaloopencexample_la_SOURCES = $(SUBSRC) analoopencexample_dict.cc
libanaloopencexample_la_DEPENDENCIES = $(SUBOBJ)
libanaloopencexample_la_LIBADD = $(SUBOBJ)
libanaloopencexample_la_LDFLAGS = $(LDFLAGS)

analoopencexample_dict.cc: $(SUBHDR) analoopencexample_linkdef.hh
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^

.cc.o:

.f.o:

.c.o:

clean-local:
	rm -f *dict*

clean:
	rm -f $(SUBOBJ)
