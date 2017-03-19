TEMPLATE = app thread
#CONFIG -= moc
INCLUDEPATH += . $(TARTSYS)/include $(MYQWTDIR)/src
LIBS += -L$(TARTSYS)/lib -L$(MYQWTDIR)/lib -lanaroot -lXMLParser -lQtXml -lQtSql -lqwt

# include "by hand" the qmake include file from
# ROOT distribution to define
# 1. include path to the ROOT system header files
# 2. the list of the ROOT shared libraries to link
# Qt application against of
# 3. qmake rules to generate ROOT/Cint dictionaries
include("$(ROOTSYS)/include/rootcint.pri")

# Input
FORMS += TAlScaler.ui TAlSelectScaler.ui
HEADERS += TAlScaler.h TAlSelectScaler.h 
SOURCES += AnaScaler.cxx TAlScaler.cxx TAlSelectScaler.cxx
RESOURCES += icon.qrc
