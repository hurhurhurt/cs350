TARGET = flip
G = getopt
CC = gcc
CFLAGS = -g -Wall -DBSD -DNDEBUG

all: $(TARGET) $(TARGET).man

$(TARGET): $(TARGET).o $(G).o
	$(CC) $(CFLAGS) $(G).o $(TARGET).o -o $(TARGET) 

$(TARGET).o: $(TARGET).h $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c

$(G).o: $(TARGET).h $(G).c
	$(CC) $(CFLAGS) -c $(G).c

$(TARGET).man:
	nroff -man flip.1 > flip.man

run: all
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) $(TARGET).man

.phony: clean

