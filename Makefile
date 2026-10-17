CC = gcc
CFLAGS = -std=c11

TARGET = code
OBJS = main3.o vector.o 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main3.o: main3.c vector.h 
	$(CC) $(CFLAGS) -c main3.c -o main3.o 

vector.o: vector.c vector.h 
	$(CC) $(CFLAGS) -c vector.c -o vector.o 

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: clean
