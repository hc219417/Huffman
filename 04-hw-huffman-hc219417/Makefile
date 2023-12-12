# Makefile for testing functions submitted with a main function
# It uses catch testing library
# Compile your program: make
# Run your program: ./huffman
#

# Variables
CFLAGS = -Wall -std=c++11
CC = g++

# Add rules to compile your program.
./huffman: huffman.cc huffman.h
	$(CC) $(CFLAGS) huffman.cc -o huffman

clean:
	rm -rf a.out *.o _TEST/NO_MAIN.cc _TEST/run_tests
