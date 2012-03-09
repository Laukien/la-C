CC = gcc
CFLAGS = -O3 -Wall
AR = ar

all: 32bit 64bit clean

32bit:
	$(CC) $(CFLAGS) -m32 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m32 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m32 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m32 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m32 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m32 -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) la_32.a *.o

64bit:
	$(CC) $(CFLAGS) -m64 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m64 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m64 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m64 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m64 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m64 -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) la_64.a *.o

clean:
	rm -f *.o
