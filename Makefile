CC = gcc

TARGET = game
SOURCES = $(wildcard src/*.c)
LINKER_FLAGS = -lSDL2 -lm

$(TARGET): $(SOURCES)
	$(CC) $^ -O3 -o $@ $(LINKER_FLAGS)

