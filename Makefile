CXX=g++
CXXFLAGS=-g -O2 -c -Wall \
         -I./ \
		 -I/usr/include \
		 -I/usr/local/include

TARGETS=libutil.a

OBJECTS= intrusive_list.o \
		singleton.o \
		time_util.o \
		url_util.o

.SUFFIXES: .o .cpp

#.cpp.o:
#    $(CXX) $(CXXFLAGS) -o $@ $<
#
#..o:
#    $(CXX) -o $@ $<

all:$(TARGETS)

libutil.a:$(OBJECTS)
	ar cr $@ $<
