VERSION=1.3.1
ARCHNAME=animmerger-$(VERSION)

ARCHDIR=archives/
ARCHFILES=\
	canvas.cc canvas.hh \
	align.cc align.hh \
	types.hh \
	settype.hh \
	maptype.hh \
	vectype.hh \
	untreetype.hh \
	pixel.cc pixel.hh pixels.hh \
	pixels/averagepixel.hh \
	pixels/changelogpixel.hh \
	pixels/lastpixel.hh \
	pixels/loopinglogpixel.hh \
	pixels/mostusedpixel.hh \
	pixelfactory.inc \
	alloc/FSBAllocator.hh \
	alloc/FSBAllocator.html \
	alloc/SmartPtr.hh \
	alloc/style.css \
	main.cc \
	\
	COPYING progdesc.php docmaker.php makediff.php \
	Makefile.sets

include Makefile.sets

OBJS=\
	main.o canvas.o pixel.o align.o
PROGS=\
	animmerger

CPPFLAGS += -I.
LDLIBS += -lgd

CXXFLAGS += -std=c++0x -fopenmp
CPPFLAGS += -DFSBALLOCATOR_USE_THREAD_SAFE_LOCKING_OPENMP

#OPTIM=-O2 -fno-inline

all: $(PROGS)

animmerger: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

makepixels: makepixels.o
	$(CXX) $(CXXFLAGS) -o $@ makepixels.o $(LDFLAGS) $(LDLIBS)

pixelfactory.inc: makepixels
	./makepixels > $@

include depfun.mak
