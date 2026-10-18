CC = gcc
CFLAGS = -std=c11 

TARGET = pot
OBJS = main.o vector.o process.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c vector.h process.h
	$(CC) $(CFLAGS) -c main.c -o main.o

vector.o: vector.c vector.h
	$(CC) $(CFLAGS) -c vector.c -o vector.o

process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c -o process.o

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
