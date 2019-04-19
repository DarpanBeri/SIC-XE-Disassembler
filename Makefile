###########################################################
#Name: Zach Selchau
#username: cssc0418
#Project: CS530 Assignment 1
#File: Makefile
#Notes: Allows for easy compilation of the program.

#Name: Darpan Beri
#username: cssc0429
#Project: CS530 Assignment 1
#File: Makefile
#Notes: Allows for easy compilation of the program.
###########################################################

#variables
CC=g++

all: xbd
	$(CC) xbd.cpp -o xbd
	export PATH="./:$PATH"

clean:
	rm -rf xbd
    
#######################[ EOF: Makefile ]###################