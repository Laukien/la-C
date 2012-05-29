CC := gcc
CFLAGS += -O3 -Wall
CFLAGS += -ggdb
CXX := g++
CXXFLAGS += -O3 -Wall
#CXXFLAGS += -ggdb
AR := ar
ARFLAGS := -rcs
NAME := lac
VERSION := 1.3
POSTGRESQL := -I$(shell pg_config --includedir-server) -I$(shell pg_config --includedir) -L$(shell pg_config --libdir) -lpq
MYSQL := $(shell mysql_config --include) $(shell mysql_config --libs)


all: cc cxx example

cc:
	@echo
	@echo === COMPILE ===
	$(CC) $(CFLAGS) -O0 -c -o la_console.o la_console.c
	$(CC) $(CFLAGS) -c -o la_database_postgresql.o la_database_postgresql.c
	$(CC) $(CFLAGS) -c -o la_database_oracle.o la_database_oracle.c
	$(CC) $(CFLAGS) -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -c -o la_directory.o la_directory.c
	$(CC) $(CFLAGS) -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -c -o la_number.o la_number.c
	$(CC) $(CFLAGS) -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -c -o la_error.o la_error.c
	$(CC) $(CFLAGS) -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(CC) $(CFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME).$(VERSION).a *.o
	gcc -shared -fPIC -Wl,-soname,lib$(NAME).$(VERSION).so -o lib$(NAME).$(VERSION).so\
		la_console.c\
		la_database_postgresql.c\
		la_database_oracle.c\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_string.c\
		la_stringbuffer.c\
		la_system.c\

cxx:
	$(CXX) $(CXXFLAGS) -O0 -c -o la_console.o la_console.c
	$(CXX) $(CXXFLAGS) -c -o la_database_postgresql.o la_database_postgresql.c
	$(CXX) $(CXXFLAGS) -c -o la_database_oracle.o la_database_oracle.c
	$(CXX) $(CXXFLAGS) -c -o la_datetime.o la_datetime.c
	$(CXX) $(CXXFLAGS) -c -o la_file.o la_file.c
	$(CXX) $(CXXFLAGS) -c -o la_directory.o la_directory.c
	$(CXX) $(CXXFLAGS) -c -o la_memory.o la_memory.c
	$(CXX) $(CXXFLAGS) -c -o la_number.o la_number.c
	$(CXX) $(CXXFLAGS) -c -o la_parameter.o la_parameter.c
	$(CXX) $(CXXFLAGS) -c -o la_error.o la_error.c
	$(CXX) $(CXXFLAGS) -c -o la_string.o la_string.c
	$(CXX) $(CXXFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(CXX) $(CXXFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME)++.$(VERSION).a *.o
	gcc -shared -fPIC -Wl,-soname,lib$(NAME)++.$(VERSION).so -o lib$(NAME)++.$(VERSION).so\
		la_console.c\
		la_database_postgresql.c\
		la_database_oracle.c\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_string.c\
		la_stringbuffer.c\
		la_system.c\

example:
	@echo
	@echo === EXAMPLE ===
	$(CC) $(CFLAGS) -o example_stringbuffer_1 example_stringbuffer_1.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_parameter_1 example_parameter_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_parameter_2 example_parameter_2.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_file_1 example_file_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_file_2 example_file_2.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_number_1 example_number_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_copyright example_copyright.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_database_postgresql_1 example_database_postgresql_1.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example_database_postgresql_2 example_database_postgresql_2.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example_database_oracle_1 example_database_oracle_1.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)

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
	cp -f lib$(NAME)++.$(VERSION).a /usr/local/lib
	ln -f -s /usr/local/lib/lib$(NAME)++.$(VERSION).a /usr/local/lib/lib$(NAME)++.a
	cp -f lib$(NAME)++.$(VERSION).so /usr/local/lib
	ln -f -s /usr/local/lib/lib$(NAME)++.$(VERSION).so /usr/local/lib/lib$(NAME)++.so
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

