# libtcod makefile for alchemy

CFLAGS=$(FLAGS) -Wall -O2
CC=gcc
CPP=g++
AR= ar rcu
RANLIB= ranlib

SRC_EXCLUDE= \
	sys_sdl_c.c \
	sys_sdl_img_bmp.c \
	sys_sdl_img_png.c \
	sys_sfml_c.c \
	zip_c.c \

SRC= $(filter-out $(SRC_EXCLUDE), $(wildcard *.c))

OBJS= $(patsubst %.c,%.o, $(SRC))

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.cpp
	$(CPP) $(CFLAGS) -c -o $@ $< 

all : libtcod.a

libtcod.a : $(OBJS)
	$(AR) $@ $?
	$(RANLIB) $@

clean :
	rm -f $(OBJS) libtcod.a libtcod.so tcod.l.bc

