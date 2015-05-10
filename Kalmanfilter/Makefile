CC=gcc-4.8
CXX=g++ -std=c++0x
RM=rm -f
CPPFLAGS=-g 
LDFLAGS=-g 
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