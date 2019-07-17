TARGET=./build/nesem
DIR=./build/
CC=g++
INC=./sdl2/include
CLFAGS=-W -Wall -Wextra
SRC=./src/*

all: $(TARGET)

$(TARGET):
	mkdir -p $(DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -I$(INC) -lpthread -Llib -lSDL2 -lSDL2main

clean:
	rm -r $(DIR)
