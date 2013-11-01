NAME := la
VERSION := 1.6.3

BINDIR := bin
OBJDIR := obj
LIBDIR := lib
PREFIX := /usr/local

CFLAGS += -O3 -Wall

#WINDOWS
ifdef WIN32
AR := i686-pc-mingw32-ar
ARFLAGS := -rcs
ifdef WITH_CPP
CC := i686-pc-mingw32-g++
CFLAGS += -static-libgcc
ARNAME := $(NAME)++.$(VERSION).lib
else
CC := i686-pc-mingw32-gcc
ARNAME := $(NAME).$(VERSION).lib
endif

#LINUX
else
CFLAGS += -ggdb3
AR := ar
ARFLAGS := -rcs
ifdef WITH_CPP
CC := g++
ARNAME := lib$(NAME)++.$(VERSION).a
SONAME := lib$(NAME)++.$(VERSION).so
else
CC := gcc
ARNAME := lib$(NAME).$(VERSION).a
SONAME := lib$(NAME).$(VERSION).so
endif

#DATABASE
ifdef WITH_MYSQL
CFLAGS += $(shell mysql_config --include)
LDFLAGS += $(shell mysql_config --libs)
endif
ifdef WITH_ORACLE
CFLAGS += -I$(ORACLE_HOME)/rdbms/public
LDFLAGS += $(ORACLE_HOME)/lib/libclient11.a -Wl,-R$(ORACLE_HOME)/lib -L$(ORACLE_HOME)/lib -lclntsh
endif
ifdef WITH_POSTGRESQL
CFLAGS += -I$(shell pg_config --includedir-server) -I$(shell pg_config --includedir)
LDFLAGS += -L$(shell pg_config --libdir) -lpq
endif
endif


all: init static dynamic

static:
	@echo
	@echo === STATIC ===
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_boolean.o src/la_boolean.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_character.o src/la_character.c
	$(CC) $(CFLAGS) -O0 -c -o $(OBJDIR)/la_console.o src/la_console.c
ifndef WIN32
ifdef WITH_POSTGRESQL
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database_postgresql.o src/la_database_postgresql.c
endif
endif
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_datetime.o src/la_datetime.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_directory.o src/la_directory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_error.o src/la_error.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_exception.o src/la_exception.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_file.o src/la_file.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_list.o src/la_list.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_memory.o src/la_memory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_message.o src/la_message.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_number.o src/la_number.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_parameter.o src/la_parameter.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_string.o src/la_string.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_stringbuffer.o src/la_stringbuffer.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_system.o src/la_system.c
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME) $(OBJDIR)/*.o
ifdef WITH_CPP
	ln -fs $(ARNAME) $(LIBDIR)/libla++.a
else
	ln -fs $(ARNAME) $(LIBDIR)/libla.a
endif

dynamic:
ifndef WIN32
	@echo
	@echo === DYNAMIC ===
	$(CC) -shared -fPIC -Wl,-soname,$(SONAME) -o $(LIBDIR)/$(SONAME)\
		src/la_boolean.c\
		src/la_character.c\
		src/la_console.c\
		src/la_datetime.c\
		src/la_directory.c\
		src/la_error.c\
		src/la_exception.c\
		src/la_file.c\
		src/la_list.c\
		src/la_memory.c\
		src/la_message.c\
		src/la_number.c\
		src/la_parameter.c\
		src/la_string.c\
		src/la_stringbuffer.c\
		src/la_system.c
ifdef WITH_POSTGRESQL
	$(CC) -shared -fPIC -Wl,-soname,lib$(NAME)-postgresql.$(VERSION).so -o $(LIBDIR)/lib$(NAME)-postgresql.$(VERSION).so src/la_database_postgresql.c
endif
endif

test:
	@echo
	@echo === TEST ===
ifdef WITH_CPP
	$(CC) $(CXXFLAGS) -I src -o example/directory_1 example/directory_1.cc $(LIBDIR)/$(ARNAME)
else
ifndef WIN32
ifdef WITH_POSTGRESQL
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_1 example/database_postgresql_1.c -L. $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_2 example/database_postgresql_2.c -L. $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_3 example/database_postgresql_3.c -L. $(LIBDIR)/$(ARNAME) $(LDFLAGS)
endif
endif
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_1 example/directory_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_2 example/directory_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/error_1 example/error_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/exception_1 example/exception_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_1 example/file_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_2 example/file_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_3 example/file_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_1 example/list_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_2 example/list_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_3 example/list_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/number_1 example/number_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_1 example/parameter_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_2 example/parameter_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_1 example/string_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_2 example/string_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_3 example/string_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_4 example/string_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/stringbuffer_1 example/stringbuffer_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_1 example/system_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_2 example/system_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
endif

init:
	@echo
	@echo ===  INIT ===
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)

clean:
	@echo
	@echo === CLEAN ===
	rm -f $(BINDIR)/*
	rm -f $(LIBDIR)/*
	rm -f $(OBJDIR)/*

install:
	@echo
	@echo === INSTALL ===
	install -o root -g root $(LIBDIR)/$(ARNAME) $(DESTDIR)$(PREFIX)/lib
ifdef WITH_CPP
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(ARNAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME)++.a
else
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(ARNAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a
endif
	install -o root -g root $(LIBDIR)/$(SONAME) $(DESTDIR)$(PREFIX)/lib
ifdef WITH_CPP
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(SONAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME)++.so
else
	ln -f -s $(DESTDIR)$(PREFIX)/lib/$(SONAME) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).so
endif
	install -o root -g root src/*.h $(DESTDIR)$(PREFIX)/include
	ldconfig $(DESTDIR)$(PREFIX)/lib

uninstall:
	@echo
	@echo === UNINSTALL ===
	rm -f $(DESTDIR)$(PREFIX)/lib/libla.*
	rm -f $(DESTDIR)$(PREFIX)/include/la_character.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_common.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_console.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_database.h
ifdef POSTGRESQL
	rm -f $(DESTDIR)$(PREFIX)/include/la_database_postgresql.h
endif
	rm -f $(DESTDIR)$(PREFIX)/include/la_datetime.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_directory.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_error.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_exception.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_file.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_list.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_memory.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_number.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_parameter.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_string.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_stringbuffer.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_system.h
	ldconfig $(DESTDIR)$(PREFIX)/lib

