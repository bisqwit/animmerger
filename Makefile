VERSION=1.5.0-pre2
ARCHNAME=animmerger-$(VERSION)

ARCHDIR=archives/
ARCHFILES=\
	palette.cc palette.hh \
	canvas.cc canvas.hh \
	align.cc align.hh \
	types.hh \
	settype.hh \
	maptype.hh \
	vectype.hh \
	binaryheap.hh \
	openmp.hh \
	kdtree.hh \
	pixel.cc pixel.hh \
	pixels/firstpixel.hh \
	pixels/lastpixel.hh \
	pixels/averagepixel.hh \
	pixels/tinyaveragepixel.hh \
	pixels/mostusedpixel.hh \
	pixels/changelogpixel.hh \
	alloc/FSBAllocator.hh \
	alloc/FSBAllocator.html \
	alloc/SmartPtr.hh \
	alloc/style.css \
	main.cc \
	\
	doc/AddingPixelMethods.txt \
	doc/docmaker.php doc/document.php \
	doc/README.html \
	\
	COPYING progdesc.php makediff.php \
	Makefile.sets

include Makefile.sets

FPOBJS=\
	fparser/fparser.o \
	fparser/fpoptimizer.o
CPPFLAGS += -Ifparser
	
OBJS=\
	main.o canvas.o pixel.o align.o palette.o
PROGS=\
	animmerger

CPPFLAGS += -I.
LDLIBS += -lgd

CXXFLAGS += -std=c++0x -fopenmp
CPPFLAGS += \
	-DFSBALLOCATOR_USE_THREAD_SAFE_LOCKING_OPENMP \
	-DFP_USE_THREAD_SAFE_EVAL

#CXXFLAGS += -m32
#CXXFLAGS += -fomit-frame-pointer
#OPTIM=-O2 -fno-inline

#CXXFLAGS += -Wno-ambiguous-bases -Wno-ambiguous-virtual-bases
# ^ Apparently, cannot be helped

#CXX=g++-4.5
#CXXFLAGS += -flto

LDLIBS += -lm

all: $(PROGS) doc/README.html

animmerger: $(OBJS) $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

animmerger_nes: main.o pixel.o align.o palette.o canvas_nes.o $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
animmerger_cga16: main.o pixel.o align.o palette.o canvas_cga16.o $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
canvas_nes.o: canvas.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(CPPFLAGS) -DNESmode=1
canvas_cga16.o: canvas.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(CPPFLAGS) -DCGA16mode=1

doc/README.html: doc/docmaker.php progdesc.php Makefile
	php -q "$<" "$(ARCHNAME)" > "$@"

include depfun.mak
