CC=g++

SRC= ../src/configparser.c
SRC+= ../src/game.c
SRC+= ../src/logger.c
SRC+= ../src/utils.c

LIBS= -I /usr/include -I /usr/src/gtest -L /usr/local/lib -lpthread -lncurses

all:
	$(CC) tests.cpp $(SRC) -o unitTest /usr/src/gtest/src/gtest_main.cc /usr/src/gtest/src/gtest-all.cc $(LIBS)
