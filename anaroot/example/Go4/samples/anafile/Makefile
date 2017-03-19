ifndef GO4SYS
GO4SYS = $(shell go4-config --go4sys)
endif

include $(GO4SYS)/Makefile.config

INCLUDES += -I$(TARTSYS)/include
LIBS_BASESET += -L$(TARTSYS)/lib -lanaroot \
               -lXMLParser
LIBS_GUISET += -L$(TARTSYS)/lib -lanaroot \
               -lXMLParser

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
EXUSERSRC_DIR         = Go4ExampleUserSource
else
EXUSERSRC_DIR         = .
endif

EXUSERSRC_LINKDEF     = $(EXUSERSRC_DIR)/ArtLinkDef.$(HedSuf)
EXUSERSRC_LIBNAME     = $(GO4_USERLIBNAME)

## must be similar for every module

EXUSERSRC_DICT        = $(EXUSERSRC_DIR)/$(DICT_PREFIX)Art
EXUSERSRC_DH          = $(EXUSERSRC_DICT).$(HedSuf)
EXUSERSRC_DS          = $(EXUSERSRC_DICT).$(SrcSuf)
EXUSERSRC_DO          = $(EXUSERSRC_DICT).$(ObjSuf)

EXUSERSRC_H           = $(filter-out $(EXUSERSRC_DH) $(EXUSERSRC_LINKDEF), $(wildcard $(EXUSERSRC_DIR)/*.$(HedSuf)))
EXUSERSRC_S           = $(filter-out $(EXUSERSRC_DS), $(wildcard $(EXUSERSRC_DIR)/*.$(SrcSuf)))
EXUSERSRC_O           = $(EXUSERSRC_S:.$(SrcSuf)=.$(ObjSuf))

EXUSERSRC_DEP         =  $(EXUSERSRC_O:.$(ObjSuf)=.$(DepSuf))
EXUSERSRC_DDEP        =  $(EXUSERSRC_DO:.$(ObjSuf)=.$(DepSuf))

EXUSERSRC_LIB         =  $(EXUSERSRC_DIR)/$(EXUSERSRC_LIBNAME).$(DllSuf)

# used in the main Makefile

EXAMPDEPENDENCS    += $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP)

ifdef DOPACKAGE
DISTRFILES         += $(EXUSERSRC_S) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)
DISTRFILES         += $(EXUSERSRC_DIR)/Readme.txt
DISTRFILES         += $(EXUSERSRC_DIR)/befoil50.scf $(EXUSERSRC_DIR)/tafoil50.scf
endif

##### local rules #####

all::  $(EXUSERSRC_LIB)

$(EXUSERSRC_LIB):   $(EXUSERSRC_O) $(EXUSERSRC_DO) $(ANAL_LIB_DEP)
	g++ -shared -Wl,-soname,libGo4UserAnalysis.so -Wl,-rpath,/opt/root/v5.34.14/lib -Wl,-rpath,/opt/go4/go4-4.7.0/lib -Wl,--no-as-needed -O ./TArtAnalysis.o ./TArtEventSource.o ./TArtParameter.o ./TArtRawEvent.o ./TArtUnpackEvent.o ./TArtProc.o ./G__Art.o -L$(TARTSYS)/lib -lanaroot -L$(ROOTSYS)/lib -lXMLParser -o libGo4UserAnalysis.so
#	@$(MakeLibrary) $(EXUSERSRC_LIBNAME) "$(EXUSERSRC_O) $(EXUSERSRC_DO)" $(EXUSERSRC_DIR) $(EXUSERSRC_LINKDEF) "$(ANAL_LIB_DEP)" $(EXUSERSRC_DS) "$(EXUSERSRC_H)"

$(EXUSERSRC_DS): $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)
	@$(ROOTCINTGO4) $(EXUSERSRC_LIB) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)

clean-bin::
	@rm -f $(EXUSERSRC_O) $(EXUSERSRC_DO)
	@rm -f $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP) $(EXUSERSRC_DS) $(EXUSERSRC_DH)

clean:: clean-bin
	@$(CleanLib) $(EXUSERSRC_LIBNAME) $(EXUSERSRC_DIR)

include $(GO4SYS)/Makefile.rules
