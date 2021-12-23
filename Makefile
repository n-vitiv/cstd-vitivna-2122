CC = gcc
CFLAGS = -g -Wall -Wpedantic -Werror -I.

SRCDIR = src

PROJ_NAME=game

MYSQL_LIBS=$(shell mysql_config --libs)
MYSQL_FLAGS=$(shell mysql_config --cflags)

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=%.o)

all :$(PROJ_NAME) 

$(PROJ_NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $(MYSQL_FLAGS) $^ -o $@ -lncurses $(MYSQL_LIBS)

$(OBJECTS) : %.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(MYSQL_FLAGS) -c $< -o $@ 

.PHONY: clean
clean :
	rm -rf $(PROJ_NAME)
