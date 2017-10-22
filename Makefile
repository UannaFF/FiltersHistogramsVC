#######################################
# Makefile PBM                        #
#                                     #
# E.B.                                #
#######################################


PROG = filters

all : $(PROG)

# Variables for file compilation
CC        =  gcc
CFLAGS    =  -g -Wall
CPPFLAGS  =  -DDEBUG
LDFLAGS   =  -g -lm

#pxmtopxm : pxmtopxm.o Util.o

filters : filters.o Util.o

clean :
	@rm -f *.o

cleanall : clean
	@rm -f $(PROG)

