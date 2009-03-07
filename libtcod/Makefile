# libtcod makefile for alchemy

CFLAGS=$(FLAGS) -Wall -O2

CC=gcc

CPP=g++

SRC_EXCLUDE= \
	sys_sdl_c.c \
	sys_sdl_img_bmp.c \
	sys_sdl_img_png.c \
	sys_sfml_c.c \
	zip_c.c \

SRC_C= $(filter-out $(SRC_EXCLUDE), $(wildcard *.c))

LIBOBJS_C= $(patsubst %.c,%.o, $(SRC_C))

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o : %.cpp
	$(CPP) $(CFLAGS) -c -o $@ $< 

all : libtcod.a

libtcod.a : $(LIBOBJS_C)
	ar r $@ $(LIBOBJS_C)

clean :
	rm -f $(LIBOBJS_C) libtcod.a libtcod.so tcod.l.bc

