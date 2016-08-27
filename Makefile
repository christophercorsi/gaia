
CXX_FLAGS=`sdl2-config --cflags` --std=c++11 -g
LIBS=`sdl2-config --libs`



gaia: gaia.cpp
	clang++ $(CXX_FLAGS) $(LIBS) gaia.cpp -o gaia

clean:
	rm -f gaia
