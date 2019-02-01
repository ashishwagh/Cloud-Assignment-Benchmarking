CC = gcc
CFLAGS = -Wall -mavx2 -lpthread -O2

clean:
	rm -rf *.o MyCPUBench

all: clean
	$(CC) $(CFLAGS) -pthread MyCPUBench.c -o MyCPUBench
