all:
	(cd zlib; ./configure; make)
	(cd libpng; make -f scripts/makefile.gcc)
	(cd libtcod; make)

clean:
	(cd zlib; make clean)
	(cd libpng; make -f scripts/makefile.gcc clean)
	(cd libtcod; make clean)
