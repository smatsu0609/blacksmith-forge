CC = g++
CFLAGS = -Wall -g

TARGET = ldec

SRCS = large_decimal.cpp

.PHONY: all clean $(TARGET)

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $@

clean:
	rm -f *.o $(TARGET)

