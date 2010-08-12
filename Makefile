VERSION=1.4.0
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
	pixel.cc pixel.hh \
	pixels/averagepixel.hh \
	pixels/tinyaveragepixel.hh \
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
	doc/AddingPixelMethods.txt \
	doc/docmaker.php doc/document.php \
	doc/README.html \
	\
	COPYING progdesc.php makediff.php \
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

all: $(PROGS) doc/README.html

animmerger: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)

doc/README.html: doc/docmaker.php progdesc.php Makefile
	php -q "$<" "$(ARCHNAME)" > "$@"

include depfun.mak
