VERSION=1.4.3
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

OBJS=\
	main.o canvas.o pixel.o align.o palette.o
PROGS=\
	animmerger

CPPFLAGS += -I.
LDLIBS += -lgd

CXXFLAGS += -std=c++0x -fopenmp
CPPFLAGS += -DFSBALLOCATOR_USE_THREAD_SAFE_LOCKING_OPENMP

#CXXFLAGS += -m32
#CXXFLAGS += -fomit-frame-pointer
#OPTIM=-O2 -fno-inline

#CXXFLAGS += -Wno-ambiguous-bases -Wno-ambiguous-virtual-bases
# ^ Apparently, cannot be helped

#CXX=g++-4.5
#CXXFLAGS += -flto

LDLIBS += -lm

all: $(PROGS) doc/README.html

animmerger: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

doc/README.html: doc/docmaker.php progdesc.php Makefile
	php -q "$<" "$(ARCHNAME)" > "$@"

include depfun.mak
