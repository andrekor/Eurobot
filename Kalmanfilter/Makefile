CC=gcc
CXX=g++-4.8 -std=c++11
RM=rm -f
CPPFLAGS=-g 
LDFLAGS=-g -Wall -Wextra -pedantic
LDLIBS=-lserial -lzmq -larmadillo
#`sdl-config --libs` -lSDL_mixer
EXEC=exec
SRCS=prog.cpp marioKalman.cpp serial.cpp 
OBJS=$(subst .cpp,.o,$(SRCS))
all: $(EXEC)

debug: CPPFLAGS += -DDEBUG -g
debug: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	$(RM) $(OBJS)
	$(RM) $(EXEC)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend