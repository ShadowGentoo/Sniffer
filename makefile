#set compiler
CC = gcc
#set compiler flags
CFLAGS = -Wall -O2
#set build dir
BUILD_DIR = ./build/
SOURCE_DIR = ./source/

all: cli daemon
cli:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)cli $(SOURCE_DIR)cli.c $(SOURCE_DIR)ui.c
daemon:
	$(CC) $(CFLAGS) -o $(BUILD_DIR)sniffd $(SOURCE_DIR)daemon.c

clean :
	rm -rf $(BUILD_DIR)*