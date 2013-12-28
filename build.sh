#!/bin/sh
set -o

DATABASE="";
#MYSQL
ls /usr/include/mysql/mysql.h &> /dev/null
if [ "$?" == "0" ]; then
	DATABASE+=" WITH_MYSQL=yes"
fi

#POSGRESQL
ls /usr/include/pgsql/server/postgres.h &> /dev/null
if [ "$?" == "0" ]; then
	DATABASE+=" WITH_POSTGRESQL=yes"
fi

#SQLITE
echo === SQLITE ===
ls /usr/include/sqlite3.h &> /dev/null
if [ "$?" == "0" ]; then
	DATABASE+=" WITH_SQLITE=yes"
fi

#COMMON
echo === COMMON ===
make
sudo make install
make WITH_CPP=yes $DATABASE
sudo make WITH_CPP=yes $DATABASE install

#MINGW
echo === WIN32 ===
which i686-pc-mingw32-gcc &> /dev/null
if [ "$?" == "0" ]; then
	echo === C - WIN32 ===
	make WIN32=yes

	echo === C++ - WIN32 ===
	make WIN32=yes WITH_CPP=yes
fi
