CC = gcc
CFLAGS = -std=c11

TARGET = pot 
OBJS = main.o vector.o 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main3.o: main.c vector.h 
	$(CC) $(CFLAGS) -c main.c -o main.o 

vector.o: vector.c vector.h 
	$(CC) $(CFLAGS) -c vector.c -o vector.o 

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: clean
