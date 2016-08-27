CXX=g++
CXX_FLAGS=`sdl2-config --cflags` --std=c++11 -O1 -g
LIBS=`sdl2-config --libs`
OUTPUT_FOLDER=build
PROGRAM=gaia

all: dummy_folders gaia

$(OUTPUT_FOLDER)/%.o : %.cpp *.h
	$(CXX) -c $(CXX_FLAGS) $< -o $@

gaia: build/gaia.o build/simulation.o build/world.o
	clang++ $(LIBS) $^ -o $(OUTPUT_FOLDER)/$(PROGRAM)

dummy_folders:
	mkdir -p $(OUTPUT_FOLDER)

clean:
	rm -rf $(OUTPUT_FOLDER)
