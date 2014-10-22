NAME := la
VERSION := `cat VERSION`
GCCVER := $(shell expr `gcc -dumpversion | cut -d '.' -f 2` \>= 8)

BINDIR := bin
OBJDIR := obj
LIBDIR := lib
PREFIX := /usr/local

#CFLAGS += -O3
#CFLAGS += -W
CFLAGS += -Wall
CFLAGS += -Werror
#CFLAGS += -pedantic
#CFLAGS += -march=nocona -mtune=generic

#WINDOWS
ifdef WIN32
EXT := .exe
CFLAGS += -D__USE_MINGW_ANSI_STDIO=1
LDFLAGS += -lws2_32 -lwsock32
AR := i686-pc-mingw32-ar
ARFLAGS := -rcs
ifdef WITH_CPP
CC := i686-pc-mingw32-g++
CFLAGS += -static-libgcc
CFLAGS += -fexceptions
ARNAME := $(NAME)++.$(VERSION).lib
ARNAME_SQLITE := $(NAME)++-sqlite.$(VERSION).lib
else
CC := i686-pc-mingw32-gcc
ARNAME := $(NAME).$(VERSION).lib
ARNAME_SQLITE := $(NAME)-sqlite.$(VERSION).lib
endif

#DATABASE
ifdef WITH_SQLITE
CFLAGS += -I ext/sqlite
LDFLAGS += ext/sqlite/sqlite3.lib
endif

#LINUX
else
EXT :=
CFLAGS += -ggdb3
LDFLAGS += -lpthread
AR := ar
ARFLAGS := -rcs
ifdef WITH_CPP
CC := g++
CFLAGS += -fexceptions
ifeq "$(GCCVER)" "1"
CFLAGS += -std=c++11
else
CFLAGS += -std=c++0x
endif
ARNAME := lib$(NAME)++.$(VERSION).a
ARNAME_MYSQL := lib$(NAME)++-mysql.$(VERSION).a
ARNAME_POSTGRESQL := lib$(NAME)++-postgresql.$(VERSION).a
ARNAME_SQLITE := lib$(NAME)++-sqlite.$(VERSION).a
SONAME := lib$(NAME)++.$(VERSION).so
SONAME_MYSQL := lib$(NAME)++-mysql.$(VERSION).so
SONAME_POSTGRESQL := lib$(NAME)++-postgresql.$(VERSION).so
SONAME_SQLITE := lib$(NAME)++-sqlite.$(VERSION).so
else
CC := gcc
ARNAME := lib$(NAME).$(VERSION).a
ARNAME_MYSQL := lib$(NAME)-mysql.$(VERSION).a
ARNAME_POSTGRESQL := lib$(NAME)-postgresql.$(VERSION).a
ARNAME_SQLITE := lib$(NAME)-sqlite.$(VERSION).a
SONAME := lib$(NAME).$(VERSION).so
SONAME_MYSQL := lib$(NAME)-mysql.$(VERSION).so
SONAME_POSTGRESQL := lib$(NAME)-postgresql.$(VERSION).so
SONAME_SQLITE := lib$(NAME)-sqlite.$(VERSION).so
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
ifdef WITH_SQLITE
CFLAGS += -I ext/sqlite
LDFLAGS += ext/sqlite/libsqlite3.a -ldl
#LDFLAGS += -lsqlite3
endif
endif


all: init static dynamic

init:
	@echo
	@echo ===  INIT ===
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(OBJDIR)
ifdef WITH_SQLITE
ifdef WIN32
	$(MAKE) -C ext/sqlite WIN32=y
else
	$(MAKE) -C ext/sqlite
endif
endif

static:
	@echo
	@echo === STATIC ===
	rm -f $(OBJDIR)/*
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_array.o src/la_array.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_atomic.o src/la_atomic.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_boolean.o src/la_boolean.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_character.o src/la_character.c
	$(CC) $(CFLAGS) -O0 -c -o $(OBJDIR)/la_console.o src/la_console.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_datetime.o src/la_datetime.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_directory.o src/la_directory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_exception.o src/la_exception.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_file.o src/la_file.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_list.o src/la_list.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_memory.o src/la_memory.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_message.o src/la_message.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_network.o src/la_network.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_number.o src/la_number.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_parameter.o src/la_parameter.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_process.o src/la_process.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_string.o src/la_string.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_stringbuffer.o src/la_stringbuffer.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_system.o src/la_system.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_thread.o src/la_thread.c
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME) $(OBJDIR)/*.o
ifdef WITH_CPP
	ln -fs $(ARNAME) $(LIBDIR)/libla++.a
else
	ln -fs $(ARNAME) $(LIBDIR)/libla.a
endif
ifdef WIN32
ifdef WITH_SQLITE
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database.o src/la_database.c -D DATABASE_SQLITE
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database_sqlite.o src/la_database_sqlite.c -D DATABASE_SQLITE
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME_SQLITE) $(OBJDIR)/la_database.o $(OBJDIR)/la_database_sqlite.o
endif
else
ifdef WITH_MYSQL
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database.o src/la_database.c -D DATABASE_MYSQL
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database_mysql.o src/la_database_mysql.c -D DATABASE_MYSQL
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME_MYSQL) $(OBJDIR)/la_database.o $(OBJDIR)/la_database_mysql.o
endif
ifdef WITH_POSTGRESQL
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database.o src/la_database.c -D DATABASE_POSTGRESQL
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database_postgresql.o src/la_database_postgresql.c -D DATABASE_POSTGRESQL
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME_POSTGRESQL) $(OBJDIR)/la_database.o $(OBJDIR)/la_database_postgresql.o
endif
ifdef WITH_SQLITE
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database.o src/la_database.c -D DATABASE_SQLITE
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/la_database_sqlite.o src/la_database_sqlite.c -D DATABASE_SQLITE
	$(AR) $(ARFLAGS) $(LIBDIR)/$(ARNAME_SQLITE) $(OBJDIR)/la_database.o $(OBJDIR)/la_database_sqlite.o
endif
endif

dynamic:
ifndef WIN32
	@echo
	@echo === DYNAMIC ===
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME) -o $(LIBDIR)/$(SONAME)\
		src/la_array.c\
		src/la_atomic.c\
		src/la_boolean.c\
		src/la_character.c\
		src/la_console.c\
		src/la_datetime.c\
		src/la_directory.c\
		src/la_exception.c\
		src/la_file.c\
		src/la_list.c\
		src/la_memory.c\
		src/la_message.c\
		src/la_network.c\
		src/la_number.c\
		src/la_parameter.c\
		src/la_process.c\
		src/la_string.c\
		src/la_stringbuffer.c\
		src/la_system.c\
		src/la_thread.c
ifdef WITH_MYSQL
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME_MYSQL) -o $(LIBDIR)/$(SONAME_MYSQL) src/la_database.c src/la_database_mysql.c -D DATABASE_MYSQL
endif
ifdef WITH_POSTGRESQL
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME_POSTGRESQL) -o $(LIBDIR)/$(SONAME_POSTGRESQL) src/la_database.c src/la_database_postgresql.c -D DATABASE_POSTGRESQL
endif
ifdef WITH_SQLITE
	$(CC) $(CFLAGS) -shared -fPIC -Wl,-soname,$(SONAME_SQLITE) -o $(LIBDIR)/$(SONAME_SQLITE) src/la_database.c src/la_database_sqlite.c -D DATABASE_SQLITE
endif
endif

test:
	@echo
	@echo === TEST ===
ifdef WITH_CPP
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/directory_1$(EXT) example/directory_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/file_1$(EXT) example/file_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/network_1$(EXT) example/network_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/parameter_1$(EXT) example/parameter_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/string_1$(EXT) example/string_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/system_1$(EXT) example/system_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CXXFLAGS) -I src -o $(BINDIR)/thread_1$(EXT) example/thread_1.cc $(LIBDIR)/$(ARNAME) $(LDFLAGS)
else
ifdef WIN32
ifdef WITH_SQLITE
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_sqlite_1$(EXT) example/database_sqlite_1.c -L. $(LIBDIR)/$(NAME)-sqlite.$(VERSION).lib $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_SQLITE
endif
else
ifdef WITH_MYSQL
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_mysql_1$(EXT) example/database_mysql_1.c -L. $(LIBDIR)/lib$(NAME)-mysql.$(VERSION).a $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_MYSQL
endif
ifdef WITH_POSTGRESQL
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_1$(EXT) example/database_postgresql_1.c -L. $(LIBDIR)/lib$(NAME)-postgresql.$(VERSION).a $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_POSTGRESQL
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_2$(EXT) example/database_postgresql_2.c -L. $(LIBDIR)/lib$(NAME)-postgresql.$(VERSION).a $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_POSTGRESQL
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_postgresql_3$(EXT) example/database_postgresql_3.c -L. $(LIBDIR)/lib$(NAME)-postgresql.$(VERSION).a $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_POSTGRESQL
endif
ifdef WITH_SQLITE
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/database_sqlite_1$(EXT) example/database_sqlite_1.c -L. $(LIBDIR)/lib$(NAME)-sqlite.$(VERSION).a $(LIBDIR)/$(ARNAME) $(LDFLAGS) -D DATABASE_SQLITE
endif
endif
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/array_1$(EXT) example/array_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/array_2$(EXT) example/array_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/atomic_1$(EXT) example/atomic_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_1$(EXT) example/directory_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/directory_2$(EXT) example/directory_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/exception_1$(EXT) example/exception_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_1$(EXT) example/file_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_2$(EXT) example/file_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/file_3$(EXT) example/file_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_1$(EXT) example/list_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_2$(EXT) example/list_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_3$(EXT) example/list_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/list_4$(EXT) example/list_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/network_1$(EXT) example/network_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/network_2$(EXT) example/network_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/memory_1$(EXT) example/memory_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/number_1$(EXT) example/number_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_1$(EXT) example/parameter_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_2$(EXT) example/parameter_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/parameter_3$(EXT) example/parameter_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/process_1$(EXT) example/process_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_1$(EXT) example/string_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_2$(EXT) example/string_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_3$(EXT) example/string_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_4$(EXT) example/string_4.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_5$(EXT) example/string_5.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/string_6$(EXT) example/string_6.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/stringbuffer_1$(EXT) example/stringbuffer_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_1$(EXT) example/system_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_2$(EXT) example/system_2.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/system_3$(EXT) example/system_3.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
	$(CC) $(CFLAGS) -I src -o $(BINDIR)/thread_1$(EXT) example/thread_1.c $(LIBDIR)/$(ARNAME) $(LDFLAGS)
endif

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
	rm -f $(DESTDIR)$(PREFIX)/include/la_console.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_database.h
ifdef WITH_POSTGRESQL
	rm -f $(DESTDIR)$(PREFIX)/include/la_database_postgresql.h
endif
	rm -f $(DESTDIR)$(PREFIX)/include/la_array.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_atomic.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_datetime.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_directory.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_exception.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_file.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_list.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_network.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_number.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_parameter.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_process.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_string.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_stringbuffer.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_system.h
	rm -f $(DESTDIR)$(PREFIX)/include/la_thread.h
	ldconfig $(DESTDIR)$(PREFIX)/lib

