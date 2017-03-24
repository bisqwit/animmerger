VERSION=1.6.2
ARCHNAME=animmerger-$(VERSION)

ARCHDIR=archives/
ARCHFILES=\
	quantize.cc quantize.hh \
	palette.cc palette.hh \
	presets.cc presets.hh \
	canvas.cc canvas.hh \
	dither.cc dither.hh \
	align.cc align.hh \
	mask.cc mask.hh \
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
	pixels/solidpixel.hh \
	alloc/FSBAllocator.hh \
	alloc/FSBAllocator.html \
	alloc/SmartPtr.hh \
	alloc/style.css \
	fparser/fparser.cc \
	fparser/fparser.hh \
	fparser/fpoptimizer.cc \
	fparser/fp_identifier_parser.inc \
	fparser/fp_opcode_add.inc \
	fparser/fpconfig.hh \
	fparser/fptypes.hh \
	fparser/fpaux.hh \
	main.cc \
	range.hh range.tcc \
	rangemap.hh rangemap.tcc \
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
	main.o canvas.o pixel.o align.o \
	palette.o quantize.o dither.o \
	mask.o presets.o
PROGS=\
	animmerger

CPPFLAGS += -I.
LDLIBS += -lgd

CXXFLAGS += -std=gnu++1z -fopenmp
CPPFLAGS += -DFSBALLOCATOR_USE_THREAD_SAFE_LOCKING_OPENMP
CPPFLAGS += -DFP_USE_THREAD_SAFE_EVAL
CPPFLAGS += -DFUNCTIONPARSER_SUPPORT_DEBUGGING

#CPPFLAGS += -pipe -fstack-protector -Wl,-z,relro -Wl,-z,now -fvisibility=hidden \
#	    -W -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unused-label \
#	    -Wpointer-arith -Wformat -Wreturn-type -Wsign-compare -Wmultichar \
#	    -Wformat-nonliteral -Winit-self -Wuninitialized -Wno-deprecated -Wformat-security \
#	    -Wno-attributes
CPPFLAGS += -pipe -W -Wall -Wextra

INSTALLPROGS = $(PROGS)
BINDIR = /usr/local/bin
INSTALL=install

#CXXFLAGS += -m32
#CXXFLAGS += -fomit-frame-pointer
#OPTIM=-O2 -fno-inline

#CXXFLAGS += -Wno-ambiguous-bases -Wno-ambiguous-virtual-bases
# ^ Apparently, cannot be helped

#CXX=g++-4.5
#CXXFLAGS += -flto

LDLIBS += -lm

all: $(PROGS)
# doc/README.html

animmerger: $(OBJS) $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

animmerger_nes: \
		main.o pixel.o align.o palette.o \
		quantize.o dither.o mask.o \
		canvas_nes.o $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
animmerger_cga16: \
		main.o pixel.o align.o palette.o \
		quantize.o dither.o mask.o \
		canvas_cga16.o $(FPOBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)
canvas_nes.o: canvas.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(CPPFLAGS) -DNESmode=1
canvas_cga16.o: canvas.cc
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(CPPFLAGS) -DCGA16mode=1

doc/README.html: doc/docmaker.php progdesc.php Makefile
	php -q "$<" "$(ARCHNAME)" > "$@"

include depfun.mak
