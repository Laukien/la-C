CC = gcc
CFLAGS += -O3 -Wall
CXX = g++
CXXFLAGS += -O3 -Wall
AR = ar
ARFLAGS = -rcs
NAME = lac
VERSION = 1.1

all: compile example

compile:
	@echo
	@echo === COMPILE ===
	$(CC) $(CFLAGS) -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -c -o la_directory.o la_directory.c
	$(CC) $(CFLAGS) -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -c -o la_number.o la_number.c
	$(CC) $(CFLAGS) -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -c -o la_error.o la_error.c
	$(CC) $(CFLAGS) -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME).$(VERSION).a *.o
	gcc -shared -fPIC -Wl,-soname,lib$(NAME).$(VERSION).so -o lib$(NAME).$(VERSION).so\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_string.c\
		la_system.c\

	$(CXX) $(CXXFLAGS) -c -o la_datetime.o la_datetime.c
	$(CXX) $(CXXFLAGS) -c -o la_file.o la_file.c
	$(CXX) $(CXXFLAGS) -c -o la_directory.o la_directory.c
	$(CXX) $(CXXFLAGS) -c -o la_memory.o la_memory.c
	$(CXX) $(CXXFLAGS) -c -o la_number.o la_number.c
	$(CXX) $(CXXFLAGS) -c -o la_parameter.o la_parameter.c
	$(CXX) $(CXXFLAGS) -c -o la_error.o la_error.c
	$(CXX) $(CXXFLAGS) -c -o la_string.o la_string.c
	$(CXX) $(CXXFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME)++.$(VERSION).a *.o
	gcc -shared -fPIC -Wl,-soname,lib$(NAME)++.$(VERSION).so -o lib$(NAME)++.$(VERSION).so\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_string.c\
		la_system.c\

	rm -f *.o

example:
	@echo
	@echo === EXAMPLE ===
	$(CC) $(CFLAGS) -o example_string_1 example_string_1.c -L. -l$(NAME).$(VERSION)
	$(CC) $(CFLAGS) -o example_parameter_1 example_parameter_1.c -L. -l$(NAME).$(VERSION)
	$(CC) $(CFLAGS) -o example_file_1 example_file_1.c -L. -l$(NAME).$(VERSION)
	$(CC) $(CFLAGS) -o example_number_1 example_number_1.c -L. -l$(NAME).$(VERSION)

clean:
	@echo
	@echo === CLEAN ===
	rm -f *.o
	rm -f *.a
	rm -f *.so

install:
	cp -f lib$(NAME).$(VERSION).a /usr/local/lib
	ln -f -s /usr/local/lib/lib$(NAME).$(VERSION).a /usr/local/lib/lib$(NAME).a
	cp -f lib$(NAME).$(VERSION).so /usr/local/lib
	ln -f -s /usr/local/lib/lib$(NAME).$(VERSION).so /usr/local/lib/lib$(NAME).so
	cp -f *.h /usr/local/include
	ldconfig /usr/local/lib

deinstall:
	rm -f /usr/local/lib/liblac.*
	rm -f /usr/local/include/la_datetime.h
	rm -f /usr/local/include/la_file.h
	rm -f /usr/local/include/la_directory.h
	rm -f /usr/local/include/la_memory.h
	rm -f /usr/local/include/la_number.h
	rm -f /usr/local/include/la_parameter.h
	rm -f /usr/local/include/la_error.h
	rm -f /usr/local/include/la_string.h
	rm -f /usr/local/include/la_system.h
	ldconfig /usr/local/lib

