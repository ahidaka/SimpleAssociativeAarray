#
#
#

TARGET = AssociativeAarray

all = $(TARGET)

CFLAGS = -g -Wall

AssociativeAarray: AssociativeAarray.c
	$(CC) $(CFLAGS) AssociativeAarray.c -o AssociativeAarray

clean:
	rm -f *.o $(TARGET) core *~
