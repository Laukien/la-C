CC := gcc
CFLAGS += -O0 -Wall
CFLAGS += -ggdb3
CXX := g++
CXXFLAGS += -O0 -Wall
CXXFLAGS += -ggdb3
GW := i686-pc-mingw32-gcc
GWFLAGS += -m32 -O0
GWXX := i686-pc-mingw32-g++
GWXXFLAGS += -m32 -O0 -static-libgcc
AR := ar
ARFLAGS := -rcs
NAME := lac
VERSION := 1.3.10
POSTGRESQL := -I$(shell pg_config --includedir-server) -I$(shell pg_config --includedir) -L$(shell pg_config --libdir) -lpq
MYSQL := $(shell mysql_config --include) $(shell mysql_config --libs)
ORACLE := -I$(ORACLE_HOME)/rdbms/public $(ORACLE_HOME)/lib/libclient11.a -Wl,-R$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/lib -lclntsh


all: cc cxx example

cc:
	@echo
	@echo === COMPILE ===
	$(CC) $(CFLAGS) -c -o la_character.o la_character.c
	$(CC) $(CFLAGS) -O0 -c -o la_console.o la_console.c
	$(CC) $(CFLAGS) -c -o la_database_postgresql.o la_database_postgresql.c
	$(CC) $(CFLAGS) -c -o la_database_oracle.o la_database_oracle.c
	$(CC) $(CFLAGS) -c -o la_datetime.o la_datetime.c
	$(CC) $(CFLAGS) -c -o la_file.o la_file.c
	$(CC) $(CFLAGS) -c -o la_directory.o la_directory.c
	$(CC) $(CFLAGS) -c -o la_list.o la_list.c
	$(CC) $(CFLAGS) -c -o la_memory.o la_memory.c
	$(CC) $(CFLAGS) -c -o la_number.o la_number.c
	$(CC) $(CFLAGS) -c -o la_parameter.o la_parameter.c
	$(CC) $(CFLAGS) -c -o la_error.o la_error.c
	$(CC) $(CFLAGS) -c -o la_exception.o la_exception.c
	$(CC) $(CFLAGS) -c -o la_string.o la_string.c
	$(CC) $(CFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(CC) $(CFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME).$(VERSION).a *.o
	$(CC) -shared -fPIC -Wl,-soname,lib$(NAME).$(VERSION).so -o lib$(NAME).$(VERSION).so\
		la_character.c\
		la_console.c\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_list.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_exception.c\
		la_string.c\
		la_stringbuffer.c\
		la_system.c
	$(CC) -shared -fPIC -Wl,-soname,lib$(NAME)-postgresql.$(VERSION).so -o lib$(NAME)-postgresql.$(VERSION).so la_database_postgresql.c

cxx:
	$(CXX) $(CXXFLAGS) -c -o la_character.o la_character.c
	$(CXX) $(CXXFLAGS) -O0 -c -o la_console.o la_console.c
	$(CXX) $(CXXFLAGS) -c -o la_database_postgresql.o la_database_postgresql.c
	$(CXX) $(CXXFLAGS) -c -o la_database_oracle.o la_database_oracle.c
	$(CXX) $(CXXFLAGS) -c -o la_datetime.o la_datetime.c
	$(CXX) $(CXXFLAGS) -c -o la_file.o la_file.c
	$(CXX) $(CXXFLAGS) -c -o la_directory.o la_directory.c
	$(CXX) $(CXXFLAGS) -c -o la_list.o la_list.c
	$(CXX) $(CXXFLAGS) -c -o la_memory.o la_memory.c
	$(CXX) $(CXXFLAGS) -c -o la_number.o la_number.c
	$(CXX) $(CXXFLAGS) -c -o la_parameter.o la_parameter.c
	$(CXX) $(CXXFLAGS) -c -o la_error.o la_error.c
	$(CXX) $(CXXFLAGS) -c -o la_exception.o la_exception.c
	$(CXX) $(CXXFLAGS) -c -o la_string.o la_string.c
	$(CXX) $(CXXFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(CXX) $(CXXFLAGS) -c -o la_system.o la_system.c
	$(AR) $(ARFLAGS) lib$(NAME)++.$(VERSION).a *.o
	$(CXX) -shared -fPIC -Wl,-soname,lib$(NAME)++.$(VERSION).so -o lib$(NAME)++.$(VERSION).so\
		la_character.c\
		la_console.c\
		la_datetime.c\
		la_file.c\
		la_directory.c\
		la_list.c\
		la_memory.c\
		la_number.c\
		la_parameter.c\
		la_error.c\
		la_exception.c\
		la_string.c\
		la_stringbuffer.c\
		la_system.c
	$(CXX) -shared -fPIC -Wl,-soname,lib$(NAME)++-postgresql.$(VERSION).so -o lib$(NAME)++-postgresql.$(VERSION).so la_database_postgresql.c

mingw-cc:
	@echo
	@echo === COMPILE ===
	$(GW) $(GWFLAGS) -c -o la_character.o la_character.c
#$(GW) $(GWFLAGS) -O0 -c -o la_console.o la_console.c
	$(GW) $(GWFLAGS) -c -o la_datetime.o la_datetime.c
	$(GW) $(GWFLAGS) -c -o la_file.o la_file.c
	$(GW) $(GWFLAGS) -c -o la_directory.o la_directory.c
	$(GW) $(GWFLAGS) -c -o la_list.o la_list.c
	$(GW) $(GWFLAGS) -c -o la_memory.o la_memory.c
	$(GW) $(GWFLAGS) -c -o la_number.o la_number.c
	$(GW) $(GWFLAGS) -c -o la_parameter.o la_parameter.c
	$(GW) $(GWFLAGS) -c -o la_error.o la_error.c
	$(GW) $(GWFLAGS) -c -o la_exception.o la_exception.c
	$(GW) $(GWFLAGS) -c -o la_string.o la_string.c
	$(GW) $(GWFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(GW) $(GWFLAGS) -c -o la_system.o la_system.c

mingw-cxx:
	$(GWXX) $(GWXXFLAGS) -c -o la_character.o la_character.c
#$(GWXX) $(GWXXFLAGS) -O0 -c -o la_console.o la_console.c
	$(GWXX) $(GWXXFLAGS) -c -o la_datetime.o la_datetime.c
	$(GWXX) $(GWXXFLAGS) -c -o la_file.o la_file.c
	$(GWXX) $(GWXXFLAGS) -c -o la_directory.o la_directory.c
	$(GWXX) $(GWXXFLAGS) -c -o la_list.o la_list.c
	$(GWXX) $(GWXXFLAGS) -c -o la_memory.o la_memory.c
	$(GWXX) $(GWXXFLAGS) -c -o la_number.o la_number.c
	$(GWXX) $(GWXXFLAGS) -c -o la_parameter.o la_parameter.c
	$(GWXX) $(GWXXFLAGS) -c -o la_error.o la_error.c
	$(GWXX) $(GWXXFLAGS) -c -o la_exception.o la_exception.c
	$(GWXX) $(GWXXFLAGS) -c -o la_string.o la_string.c
	$(GWXX) $(GWXXFLAGS) -c -o la_stringbuffer.o la_stringbuffer.c
	$(GWXX) $(GWXXFLAGS) -c -o la_system.o la_system.c

example:
	@echo
	@echo === EXAMPLE ===
	$(CC) $(CFLAGS) -o example_directory_1 example_directory_1.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_directory_2 example_directory_2.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_string_1 example_string_1.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_string_2 example_string_2.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_string_3 example_string_3.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_string_4 example_string_4.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_stringbuffer_1 example_stringbuffer_1.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_system_1 example_system_1.c lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_list_1 example_list_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_list_2 example_list_2.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_list_3 example_list_3.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_parameter_1 example_parameter_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_parameter_2 example_parameter_2.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_file_1 example_file_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_file_2 example_file_2.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_file_3 example_file_3.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_number_1 example_number_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_copyright_1 example_copyright_1.c -L. lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example_database_postgresql_1 example_database_postgresql_1.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example_database_postgresql_2 example_database_postgresql_2.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example_database_postgresql_3 example_database_postgresql_3.c -L. lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example_exception_1 example_exception_1.c -L. lib$(NAME).$(VERSION).a

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

