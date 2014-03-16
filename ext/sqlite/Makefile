CFLAGS += -Wall
CFLAGS += -Werror
#CFLAGS += -O3

#WINDOWS
ifdef WIN32
CFLAGS += -D__USE_MINGW_ANSI_STDIO=1
CC := i686-pc-mingw32-gcc
AR := i686-pc-mingw32-ar
ARFLAGS := -rcs
ARNAME := sqlite3.lib
else
CFLAGS += -ggdb3
CC := gcc
AR := ar
ARFLAGS := -rcs
ARNAME := libsqlite3.a
endif

all:
	perl download.pl
	unzip -o sqlite.zip
	cp sqlite-*/sqlite3.h .
	cp sqlite-*/sqlite3.c .
	$(CC) $(CFLAGS) -c -o sqlite3.o sqlite3.c
	$(AR) $(ARFLAGS) $(ARNAME) sqlite3.o
	rm sqlite3.o

