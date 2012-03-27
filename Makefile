CC = gcc
CFLAGS += -O3 -Wall
CXX = g++
CXXFLAGS += -O3 -Wall
AR = ar
ARFLAGS = -rcs

all: 32bit 64bit example clean

32bit:
	@echo 32 BIT
	$(CC) $(CFLAGS) -m32 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m32 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m32 -c -o la_directory.o la_directory.c
	$(CC) $(CFLAGS) -m32 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m32 -c -o la_number.o la_number.c
	$(CC) $(CFLAGS) -m32 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m32 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m32 -c -o la_system.o la_system.c
	$(CXX) $(CXXFLAGS) -m32 -c -o la_wrapper.o la_wrapper.cc
	$(AR) $(ARFLAGS) la_32.a *.o

64bit:
	@echo 64 BIT
	$(CC) $(CFLAGS) -m64 -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -m64 -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -m64 -c -o la_directory.o la_directory.c
	$(CC) $(CFLAGS) -m64 -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -m64 -c -o la_number.o la_number.c
	$(CC) $(CFLAGS) -m64 -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -m64 -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -m64 -c -o la_system.o la_system.c
	$(CXX) $(CXXFLAGS) -m64 -c -o la_wrapper.o la_wrapper.cc
	$(AR) $(ARFLAGS) la_64.a *.o

example:
	@echo EXAMPLE
	$(CC) $(CFLAGS) -m32 -o example_string_1 example_string_1.c la_32.a
	$(CC) $(CFLAGS) -m32 -o example_parameter_1 example_parameter_1.c la_32.a
	$(CC) $(CFLAGS) -m32 -o example_file_1 example_file_1.c la_32.a
	$(CC) $(CFLAGS) -m32 -o example_number_1 example_number_1.c la_32.a

clean:
	@echo CLEAN
	rm -f *.o
