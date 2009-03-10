.SUFFIXES: .cpp .c .o

NAME=libtcod-alchemy
WRAPPER=libtcod_wrapper
PATH:=${ALCHEMY_HOME}/achacks:${PATH}
PACKAGE_WRAPPER=libtcodAlchemy
WRAPPER_SRC_DIR=actionscript

LIB_DIR=../lib

INC_LIBTCOD = -I $(LIB_DIR)/libtcod
LD_LIBTCOD = -L=$(LIB_DIR)/libtcod -l=tcod

INC_LIBPNG = -I $(LIB_DIR)/libpng
LD_LIBPNG = -L$(LIB_DIR)/libpng -lpng

INC_ZLIB = -I $(LIB_DIR)/zlib
LD_ZLIB = -L$(LIB_DIR)/zlib -lz

GG = gluegen
INC = $(INC_LIBTCOD) $(INC_LIBPNG) $(INC_ZLIB)
CC = gcc
CCPP = g++
CFLAGS = -DTERMIOS -DL_ENDIAN -fomit-frame-pointer -O3 -Wall
LDFLAGS =  $(LD_LIBTCOD) $(LD_LIBPNG) $(LD_ZLIB)

SRC_EXCLUDE = glue.c

SRC = $(filter-out $(SRC_EXCLUDE), $(wildcard *.c))

OBJS = $(patsubst %.c,%.o, $(SRC))

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

all: $(NAME).swc

glue.c : $(WRAPPER).gg
	$(GG) $(WRAPPER).gg -oas $(WRAPPER_SRC_DIR)/$(PACKAGE_WRAPPER)/$(WRAPPER).as -cpackage cmodule.$(WRAPPER) -package $(PACKAGE_WRAPPER) -class $(WRAPPER)

glue.o : glue.c

$(WRAPPER).swc : glue.o $(OBJS)
	$(CC) $(INC) $(CFLAGS) -swc -o $(WRAPPER).swc glue.o $(OBJS) $(LDFLAGS)

$(NAME).swc : $(WRAPPER).swc
	compc -namespace $(PACKAGE_WRAPPER) manifest.xml -source-path $(WRAPPER_SRC_DIR) -include-namespaces $(PACKAGE_WRAPPER) -include-libraries $(WRAPPER).swc -target-player=10.0.0 -output='$(NAME).swc'

$(WRAPPER).gg : 

clean:
	rm $(OBJS)
	rm glue.*
	rm $(WRAPPER).swc 
	rm $(NAME).swc
	rm *.o
	rm $(WRAPPER_SRC_DIR)/$(WRAPPER).as
	rm swfbridge.log 
	rm *achacks*
	rm -r _sb_*
	rm -rf bin
