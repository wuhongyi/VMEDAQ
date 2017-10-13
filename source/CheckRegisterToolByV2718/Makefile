########################################################################
#                                                                      
#              --- CAEN SpA - Computing Division ---                   
#                                                                      
#   CAENVMElib Software Project                                        
#                                                                      
#   Created  :  March 2004      (Rel. 1.0)                                             
#                                                                      
#   Auth: S. Coluccini                                                 
#                                                                      
########################################################################

EXEWU   =       pku

GCC	=	g++

COPTS	=	-fPIC -DLINUX -Wall 
#COPTS	=	-g -fPIC -DLINUX -Wall 

FLAGS	=	-Wall -s
#FLAGS	=	-Wall

DEPLIBS	=       -l CAENVME -l curses -lc -lm

LIBS	=	

INCLUDEDIR =	-I.

OBJSWU	=       main.o

INCLUDES =      

#########################################################################

all	:	$(EXEWU)

clean	:
		/bin/rm -f *.o $(EXEWU)


$(EXEWU) :	$(OBJSWU)
		$(GCC) -std=c++11 $(OBJSWU) $(DEPLIBS) -o $(EXEWU)

$(OBJS)	:	$(INCLUDES) Makefile


%.o	:	%.cc
		$(GCC) -std=c++11 $(COPTS)  -c -o $@ $<
