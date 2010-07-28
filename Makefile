VERSION=1.0.0
ARCHNAME=animmerger-$(VERSION)

ARCHDIR=archives/
ARCHFILES=\
	canvas.cc canvas.hh \
	maptype.hh \
	vectype.hh \
	pixel.cc pixel.hh \
	pixels/averagepixel.hh \
	pixels/changelogpixel.hh \
	pixels/lastpixel.hh \
	pixels/loopinglogpixel.hh \
	pixels/mostusedpixel.hh \
	pixels/mostusedwithinpixel.hh \
	alloc/FSBAllocator.hh \
	alloc/FSBAllocator.html \
	alloc/SmartPtr.hh \
	alloc/style.css \
	main.cc \
	\
	Makefile.sets

include Makefile.sets

OBJS=\
	main.o canvas.o pixel.o
PROGS=\
	animmerger

CPPFLAGS += -I.
LDLIBS += -lgd

all: $(PROGS)

animmerger: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)


include depfun.mak
