TEMPLATE = app thread
#CONFIG -= moc
INCLUDEPATH += . $(TARTSYS)/include
LIBS += -L$(TARTSYS)/lib -lanaroot -lXMLParser -lQtXml

# include "by hand" the qmake include file from
# ROOT distribution to define
# 1. include path to the ROOT system header files
# 2. the list of the ROOT shared libraries to link
# Qt application against of
# 3. qmake rules to generate ROOT/Cint dictionaries
include("$(ROOTSYS)/include/rootcint.pri")

# Input
FORMS += TAlMainWindow.ui
HEADERS += TAlMainWindow.h
SOURCES += AnaLoop.cxx TAlMainWindow.cxx
