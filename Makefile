#CC = gcc
CFLAGS += -O3 -Wall
#AR = ar

all: 32bit 64bit example clean

32bit:
	@echo 32 BIT
	$(CC) $(CFLAGS) -m32 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m32 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m32 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m32 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m32 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m32 -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) la_32.a *.o

64bit:
	@echo 64 BIT
	$(CC) $(CFLAGS) -m64 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m64 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m64 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m64 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m64 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m64 -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) la_64.a *.o

example:
	@echo EXAMPLE
	$(CC) $(CFLAGS) -m32 -o example_string_1 example_string_1.c la_32.a

clean:
	@echo CLEAN
	rm -f *.o
