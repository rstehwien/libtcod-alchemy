.SUFFIXES: .cpp .c .o

NAME=libtcod-alchemy-wrapper
WRAPPER=libtcod_wrapper
PATH:=${ALCHEMY_HOME}/achacks:${PATH}
LIBTCOD_SRC=../libtcod
PACKAGE_WRAPPER=libtcodAlchemy
WRAPPER_SRC_DIR=./bin/src
GG = gluegen
INC = -I $(LIBTCOD_SRC)
CC = gcc
CCPP = g++
CFLAGS = -DTERMIOS -DL_ENDIAN -fomit-frame-pointer -O3 -Wall
LDFLAGS = -L=$(LIBTCOD_SRC) -l=tcod

SRC_EXCLUDE = glue.c

SRC = $(filter-out $(SRC_EXCLUDE), $(wildcard *.c))

OBJS = $(patsubst %.c,%.o, $(SRC))

%.o : %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

all: $(NAME).swc

glue.c : $(WRAPPER).gg
	$(GG) $(WRAPPER).gg -oas $(WRAPPER).as -cpackage cmodule.$(WRAPPER) -package $(PACKAGE_WRAPPER) -class $(WRAPPER)
	mkdir -p $(WRAPPER_SRC_DIR)/$(PACKAGE_WRAPPER)
	cp $(WRAPPER).as $(WRAPPER_SRC_DIR)/$(PACKAGE_WRAPPER)

glue.o : glue.c

$(WRAPPER).swc : glue.o $(OBJS)
	$(CC) $(INC) $(CFLAGS) -swc -o $(WRAPPER).swc glue.o $(OBJS) $(LDFLAGS)

$(NAME).swc : $(WRAPPER).swc
	compc -namespace $(PACKAGE_WRAPPER) manifest.xml -source-path $(WRAPPER_SRC_DIR) -include-namespaces $(PACKAGE_WRAPPER) -include-classes $(PACKAGE_WRAPPER).$(WRAPPER) -include-libraries $(WRAPPER).swc -target-player=10.0.0 -output='$(NAME).swc'

$(WRAPPER).gg : 

clean:
	rm $(OBJS) glue.* $(WRAPPER).swc $(WRAPPER).as $(NAME).swc *.o
	rm swfbridge.log 
	rm *achacks*
	rm -r _sb_*
	rm -rf bin
