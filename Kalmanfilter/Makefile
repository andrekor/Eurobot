OBJS = mario
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LDLIBS = -larmadillo -lzmq
RM = rm -f *.o
SRC = marioKalman.h marioKalman.cpp

all: $(SRC)
	$(CC) marioKalman.cpp -o $(OBJS) $(LDLIBS) 


clean:
	$(RM) $(OBJS)