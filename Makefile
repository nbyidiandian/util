CXX=g++
CXXFLAGS=-g -O2 -c -Wall \
         -D__STDC_LIMIT_MACROS \
         -I./ \
		 -I/usr/include \
		 -I/usr/local/include

TARGETS=libutil.a

OBJECTS=intrusive_list.o \
		singleton.o \
		time_util.o \
		scoped_latency.o \
		url_util.o

.SUFFIXES: .o .cpp

#.cpp.o:
#    $(CXX) $(CXXFLAGS) -o $@ $<
#
#..o:
#    $(CXX) -o $@ $<

all:$(TARGETS)

libutil.a:$(OBJECTS)
	echo $^
	$(AR) scr $@ $^

.PHONY:clean
clean:
	rm -f $(TARGETS)
	rm -f $(OBJECTS)
