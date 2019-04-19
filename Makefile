###########################################################
#Name: Blake Meyers(cssc0430, Red id: 819557369), Darpan Beri(cssc0429, Red id: 820880704), Zach Selchau(cssc0418, Red id: 820533188)
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