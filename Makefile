all:
	(cd zlib; make)
	(cd libpng; make -f scripts/makefile.gcc)
	(cd libtcod; make)
	(cd wrapper; make)

clean:
	(cd zlib; make clean)
	(cd libpng; make -f scripts/makefile.gcc clean)
	(cd libtcod; make clean)
	(cd wrapper; make clean)
