CC = clang
CFLAGS = -std=c99 -Wall
LDFLAGS = -lraylib -lm
SRCS = main.c
TARGET = main

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)