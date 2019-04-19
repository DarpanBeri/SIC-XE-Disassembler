###########################################################
#Name: Blake Meyers(cssc0430), Darpan Beri(cssc0429), Zach Selchau(cssc0418)
#Project: CS530 Assignment 2
#File: Makefile
#Notes: Allows for easy compilation of the program.
###########################################################

#variables
CC=g++

all: xed
	$(CC) xed.cpp -o xed
	export PATH="./:$PATH"

clean:
	rm -rf xed
	rm -rf sample.sic
	rm -rf sample.lis
    
#######################[ EOF: Makefile ]###################