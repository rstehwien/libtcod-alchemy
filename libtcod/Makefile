# libtcod makefile
# this alchemy makefile excludes any expernal libraries (zlib, sdl, png)
# instead of libraries from the dependencies directory
# might be useful if you're using a libc version not compatible
# to build debug version, run make FLAGS=-g
# to build release version, run make FLAGS=-O2 -s
BINDIR=.
SRCDIR=.
INCDIR=.

#dependencies
# SDL
SDL_LIBS=

# ZLIB
ZLIB_LIBS=

# PNG
PNG_LIBS=


CFLAGS=$(FLAGS) -Wall -O2
TEMP=.
CC=gcc
CPP=g++
.SUFFIXES: a .o .h .c

%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<
%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $< 
%.o : $(SRCDIR)/gui/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $< 


LIBOBJS_C= \
	bresenham_c.o \
	bsp_c.o \
	color_c.o \
	console_c.o \
	fov_c.o \
	fov_circular_raycasting.o \
	fov_diamond_raycasting.o \
	fov_recursive_shadowcasting.o \
	fov_digital.o \
	fov_permissive2.o \
	heightmap_c.o \
	image_c.o \
	lex_c.o \
	list_c.o \
	mersenne_c.o \
	noise_c.o \
	parser_c.o \
	path_c.o \
	sys_c.o \
#	sys_sdl_c.o \
#	sys_sdl_img_bmp.o \
#	sys_sdl_img_png.o \
	tree_c.o \
	wrappers.o \
#	zip_c.o

all : libtcod.a 

libtcod.a : $(LIBOBJS_C)
	ar r $@ $(LIBOBJS_C)

clean : 
	\rm -f $(LIBOBJS_C) $(LIBOBJS_CPP) $(GUIOBJS) libtcod.a libtcod++.a libtcod.so libtcod++.so libtcod-gui.so libtcodpy.pyc *.o

