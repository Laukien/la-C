CC := gcc
CFLAGS += -O3 -Wall
CFLAGS += -ggdb3
CXX := g++
CXXFLAGS += -O3 -Wall
CXXFLAGS += -ggdb3
WIN := true
GW := i686-pc-mingw32-gcc
GWFLAGS += -m32 -O3
GWXX := i686-pc-mingw32-g++
GWXXFLAGS += -m32 -O3 -static-libgcc
AR := ar
ARFLAGS := -rcs
NAME := la
VERSION := 1.4.0
POSTGRESQL := -I$(shell pg_config --includedir-server) -I$(shell pg_config --includedir) -L$(shell pg_config --libdir) -lpq
#MYSQL := $(shell mysql_config --include) $(shell mysql_config --libs)
#ORACLE := -I$(ORACLE_HOME)/rdbms/public $(ORACLE_HOME)/lib/libclient11.a -Wl,-R$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/lib -lclntsh


all: clean cc cxx cc-example

cc: cc-static cc-dynamic cc-win

cxx: cxx-static cc-dynamic cxx-win

cc-static:
	@echo
	@echo === CC-STATIC ===
	$(CC) $(CFLAGS) -c -o bin/la_character.o src/la_character.c
	$(CC) $(CFLAGS) -O0 -c -o bin/la_console.o src/la_console.c
ifdef POSTGRESQL
	$(CC) $(CFLAGS) -c -o bin/la_database_postgresql.o src/la_database_postgresql.c
endif
	$(CC) $(CFLAGS) -c -o bin/la_datetime.o src/la_datetime.c
	$(CC) $(CFLAGS) -c -o bin/la_directory.o src/la_directory.c
	$(CC) $(CFLAGS) -c -o bin/la_error.o src/la_error.c
	$(CC) $(CFLAGS) -c -o bin/la_exception.o src/la_exception.c
	$(CC) $(CFLAGS) -c -o bin/la_file.o src/la_file.c
	$(CC) $(CFLAGS) -c -o bin/la_list.o src/la_list.c
	$(CC) $(CFLAGS) -c -o bin/la_memory.o src/la_memory.c
	$(CC) $(CFLAGS) -c -o bin/la_number.o src/la_number.c
	$(CC) $(CFLAGS) -c -o bin/la_parameter.o src/la_parameter.c
	$(CC) $(CFLAGS) -c -o bin/la_string.o src/la_string.c
	$(CC) $(CFLAGS) -c -o bin/la_stringbuffer.o src/la_stringbuffer.c
	$(CC) $(CFLAGS) -c -o bin/la_system.o src/la_system.c
	$(AR) $(ARFLAGS) bin/lib$(NAME).$(VERSION).a bin/*.o

cc-dynamic:
	@echo
	@echo === CC-DYNAMIC ===
	$(CC) -shared -fPIC -Wl,-soname,lib$(NAME).$(VERSION).so -o bin/lib$(NAME).$(VERSION).so\
		src/la_character.c\
		src/la_console.c\
		src/la_datetime.c\
		src/la_directory.c\
		src/la_error.c\
		src/la_exception.c\
		src/la_file.c\
		src/la_list.c\
		src/la_memory.c\
		src/la_number.c\
		src/la_parameter.c\
		src/la_string.c\
		src/la_stringbuffer.c\
		src/la_system.c
ifdef POSTGRESQL
	$(CC) -shared -fPIC -Wl,-soname,lib$(NAME)-postgresql.$(VERSION).so -o bin/lib$(NAME)-postgresql.$(VERSION).so src/la_database_postgresql.c
endif

cxx-static:
	@echo
	@echo === CXX-STATIC ===
	$(CXX) $(CXXFLAGS) -c -o bin/la_character.o src/la_character.c
	$(CXX) $(CXXFLAGS) -O0 -c -o bin/la_console.o src/la_console.c
ifdef POSTGRESQL
	$(CXX) $(CXXFLAGS) -c -o bin/la_database_postgresql.o src/la_database_postgresql.c
endif
	$(CXX) $(CXXFLAGS) -c -o bin/la_datetime.o src/la_datetime.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_directory.o src/la_directory.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_error.o src/la_error.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_exception.o src/la_exception.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_file.o src/la_file.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_list.o src/la_list.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_memory.o src/la_memory.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_number.o src/la_number.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_parameter.o src/la_parameter.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_string.o src/la_string.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_stringbuffer.o src/la_stringbuffer.c
	$(CXX) $(CXXFLAGS) -c -o bin/la_system.o src/la_system.c
	$(AR) $(ARFLAGS) bin/lib$(NAME)++.$(VERSION).a bin/*.o

cxx-dynamic:
	@echo
	@echo === CXX-DYNAMIC ===
	$(CXX) -shared -fPIC -Wl,-soname,lib$(NAME)++.$(VERSION).so -o bin/lib$(NAME)++.$(VERSION).so\
		src/la_character.c\
		src/la_console.c\
		src/la_datetime.c\
		src/la_directory.c\
		src/la_error.c\
		src/la_exception.c\
		src/la_file.c\
		src/la_list.c\
		src/la_memory.c\
		src/la_number.c\
		src/la_parameter.c\
		src/la_string.c\
		src/la_stringbuffer.c\
		src/la_system.c
ifdef POSTGRESQL
	$(CXX) -shared -fPIC -Wl,-soname,lib$(NAME)++-postgresql.$(VERSION).so -o bin/lib$(NAME)++-postgresql.$(VERSION).so src/la_database_postgresql.c
endif

cc-win:
	@echo
	@echo === CC-WIN ===
ifdef WIN
	$(GW) $(GWFLAGS) -c -o bin/la_character.o src/la_character.c
	$(GW) $(GWFLAGS) -O0 -c -o bin/la_console.o src/la_console.c
	$(GW) $(GWFLAGS) -c -o bin/la_datetime.o src/la_datetime.c
	$(GW) $(GWFLAGS) -c -o bin/la_directory.o src/la_directory.c
	$(GW) $(GWFLAGS) -c -o bin/la_error.o src/la_error.c
	$(GW) $(GWFLAGS) -c -o bin/la_exception.o src/la_exception.c
	$(GW) $(GWFLAGS) -c -o bin/la_file.o src/la_file.c
	$(GW) $(GWFLAGS) -c -o bin/la_list.o src/la_list.c
	$(GW) $(GWFLAGS) -c -o bin/la_memory.o src/la_memory.c
	$(GW) $(GWFLAGS) -c -o bin/la_number.o src/la_number.c
	$(GW) $(GWFLAGS) -c -o bin/la_parameter.o src/la_parameter.c
	$(GW) $(GWFLAGS) -c -o bin/la_string.o src/la_string.c
	$(GW) $(GWFLAGS) -c -o bin/la_stringbuffer.o src/la_stringbuffer.c
	$(GW) $(GWFLAGS) -c -o bin/la_system.o src/la_system.c
else
	@echo nothing to compile
endif

cxx-win:
	@echo
	@echo === CXX-WIN ===
ifdef WIN
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_character.o src/la_character.c
	$(GWXX) $(GWXXFLAGS) -O0 -c -o bin/la_console.o src/la_console.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_datetime.o src/la_datetime.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_directory.o src/la_directory.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_error.o src/la_error.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_exception.o src/la_exception.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_file.o src/la_file.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_list.o src/la_list.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_memory.o src/la_memory.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_number.o src/la_number.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_parameter.o src/la_parameter.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_string.o src/la_string.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_stringbuffer.o src/la_stringbuffer.c
	$(GWXX) $(GWXXFLAGS) -c -o bin/la_system.o src/la_system.c
else
	@echo nothing to compile
endif

cc-example:
	@echo
	@echo === CC-EXAMPLE ===
ifdef POSTGRESQL
	$(CC) $(CFLAGS) -o example/database_postgresql_1 example/database_postgresql_1.c -L. bin/lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example/database_postgresql_2 example/database_postgresql_2.c -L. bin/lib$(NAME).$(VERSION).a $(POSTGRESQL)
	$(CC) $(CFLAGS) -o example/database_postgresql_3 example/database_postgresql_3.c -L. bin/lib$(NAME).$(VERSION).a $(POSTGRESQL)
endif
	$(CC) $(CFLAGS) -o example/directory_1 example/directory_1.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/directory_2 example/directory_2.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/exception_1 example/exception_1.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/file_1 example/file_1.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/file_2 example/file_2.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/file_3 example/file_3.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/list_1 example/list_1.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/list_2 example/list_2.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/list_3 example/list_3.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/number_1 example/number_1.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/parameter_1 example/parameter_1.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/parameter_2 example/parameter_2.c -L. bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/string_1 example/string_1.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/string_2 example/string_2.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/string_3 example/string_3.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/string_4 example/string_4.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/stringbuffer_1 example/stringbuffer_1.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/system_1 example/system_1.c bin/lib$(NAME).$(VERSION).a
	$(CC) $(CFLAGS) -o example/system_2 example/system_2.c -L. bin/lib$(NAME).$(VERSION).a

clean:
	@echo
	@echo === CLEAN ===
	mkdir -p bin
	rm -f bin/*

install:
	@echo
	@echo === INSTALL ===
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
	@echo
	@echo === DEINSTALL ===
	rm -f /usr/local/lib/liblac.*
	rm -f /usr/local/include/la_character.h
	rm -f /usr/local/include/la_common.h
	rm -f /usr/local/include/la_console.h
	rm -f /usr/local/include/la_database.h
ifdef POSTGRESQL
	rm -f /usr/local/include/la_database_postgresql.h
endif
	rm -f /usr/local/include/la_datetime.h
	rm -f /usr/local/include/la_directory.h
	rm -f /usr/local/include/la_error.h
	rm -f /usr/local/include/la_exception.h
	rm -f /usr/local/include/la_file.h
	rm -f /usr/local/include/la_list.h
	rm -f /usr/local/include/la_memory.h
	rm -f /usr/local/include/la_number.h
	rm -f /usr/local/include/la_parameter.h
	rm -f /usr/local/include/la_string.h
	rm -f /usr/local/include/la_stringbuffer.h
	rm -f /usr/local/include/la_system.h
	ldconfig /usr/local/lib

