###########################################################
#Name: Blake Meyers, Darpan Beri, Zach Selchau
#Project: SIC-XE-Disassembler
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
