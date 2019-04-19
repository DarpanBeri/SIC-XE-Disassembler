/*************************************************************
 Name: Blake Meyers(cssc0430), Darpan Beri(cssc0429), Zach Selchau(cssc0418) 
 Project: CS530 Assignment 2
 File: xed.hpp
 Notes: Header file containing required libraries and 
 function prototypes.
 *************************************************************/
#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>

#define nullptr NULL

using namespace std;


void gracefulExit(string msg);
bool fileExists(string filename);
bool fileExists(const char* filename);
FILE* createFile(string filename);
bool closeFile(FILE* fp);
int hexToDecimal(string num);
int signedHexToDecimal(string num);
string decimalToHex(int num);
string toString(int num);
string nixbpeFinder(string hex);
bool opcodeValid(string hex);
int formatFinder(string hex);
string hexToCommand(string hex);

class Symbol{
    private:
        string name;
        char flag;
        string value;
        int decValue;

    public:
        Symbol *next;
        Symbol(string nam, string val, char flg, Symbol *nextSym);
        char getFlag();
        string getValue();
        int getDecValue();
        void setNext(Symbol *nextSym);
        string getName();
};

class Literal{
    private:
        string name;
        string address;
        string length;
        int decAddress;
        int decLength;

    public:
        Literal *next;
        Literal(string nam, string addy, string len, Literal *nextLit);
        string getAddress();
        string getLength();
        int getDecAddress();
        int getDecLength();
        void setNext(Literal *nextLit);
        string getName();
};

Symbol* findAddressInSymtab(Symbol *symPtr, string address);
Literal* findAddressInLittab(Literal *litPtr, string address);
Symbol* toSymbol(Symbol* head, FILE *fp);
Literal* toLiteral(Literal* &head, FILE *fp);
vector<string> readObj(FILE *fp, Symbol *symHead, Literal *litHead);
void writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead);
void writeAddress(FILE *fp, int address);
void writeOpcode(FILE *fp, string opcode, int column);
void writeLisFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead);

/**************************[ EOF: xbd.hpp ]************************/