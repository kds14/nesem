TARGET=./build/nesem
DIR=./build/
CC=g++
CFLAGS=-std=c++17 -g
SRC=./src/*.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(shell sdl2-config --cflags --libs)

clean:
	rm -r $(DIR)
