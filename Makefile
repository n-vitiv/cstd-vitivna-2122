CC = gcc
CFLAGS = -g -Wall -Wpedantic -Werror -I.

SRCDIR = src

PROJ_NAME=game

SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=%.o)

all :$(PROJ_NAME) 

$(PROJ_NAME) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

$(OBJECTS) : %.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean :
	rm -rf $(PROJ_NAME)
