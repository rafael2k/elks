# makefile for Levee

BASEDIR=..

include $(BASEDIR)/Makefile-rules

###############################################################################

LOCALFLAGS=-DSIZE=16000L

###############################################################################

OBJS = blockio.o display.o editcor.o exec.o find.o \
    unixcall.o globals.o insert.o main.o misc.o \
    modify.o move.o ucsd.o undo.o wildargs.o

all: lev

lev: $(OBJS)
	$(LD) $(LDFLAGS) -o lev $(OBJS) $(LDLIBS)

install: lev
	$(INSTALL) lev $(DESTDIR)/bin/vi

clean:
	$(RM) *.o lev

# Dependencies

blockio.o    : levee.h extern.h blockio.c
display.o    : levee.h extern.h termcap.i display.c
editcor.o    : levee.h extern.h editcor.c
exec.o       : levee.h extern.h exec.c
find.o       : levee.h extern.h grep.h find.c
globals.o    : levee.h globals.c
insert.o     : levee.h extern.h insert.c
main.o       : levee.h extern.h main.c
misc.o       : levee.h extern.h misc.c
modify.o     : levee.h extern.h grep.h modify.c
move.o       : levee.h extern.h move.c
rmxcall.o    : levee.h rmxcall.c
ucsd.o       : levee.h extern.h ucsd.c
undo.o       : levee.h extern.h undo.c
