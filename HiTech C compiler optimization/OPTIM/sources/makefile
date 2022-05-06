########################################################################
#
#  Use this Makefile to build the CGEN for HiTech C v3.09 under Linux
#  using John Elliott's zxcc emulator.
#
########################################################################

VERSION = 3.0

CSRCS =	optim1.c \
	part21a.c \
	part21b.c \
	part21c.c \
	part21c2.c \
	part21d.c \
	part31a.c \
	part31b.c \
	part31c.c \
	part41a.c \
	part41b.c \
	ctype1.c \
	initvar1.c

ASRCS = mylibc.as \
	mycrtcpm.as \
	myalloc.as

COBJS = $(CSRCS:.c=.obj)
AOBJS = $(ASRCS:.as=.obj)

OBJS = $(COBJS) $(AOBJS)

all:	$(COBJS) $(AOBJS) optim1.com 

.SUFFIXES:		# delete the default suffixes
.SUFFIXES: .c .obj

$(COBJS): %.obj: %.c
	zxc -c -o  $<

$(AOBJS): %.obj: %.as
	zxas -n $<
#	zxas -j -n $<

optim1.com: $(OBJS)
	zxlink <lkoptim
	sort <optim1.sym | uniq > optim1.sym.sorted

clean:
	rm -f $(OBJS) optim1.com *.\$$\$$\$$ optim1.map optim1.sym optim1.sym.sorted

