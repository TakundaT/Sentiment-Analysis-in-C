
CC=gcc
CFLAGS=-Wall -g
TARGET=mySA


all: $(TARGET)

$(TARGET): mySA.c
	$(CC) $(CFLAGS) -o $(TARGET) mySA.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
