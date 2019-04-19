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

/*************************************************************
 FUNCTION: gracefulExit()
 DESCRIPTION: Exits gracefully
 I/O:
    input parameters: String
    output: Void.
 *************************************************************/
void gracefulExit(string msg);

/*************************************************************
 FUNCTION: fileExists()
 DESCRIPTION: Check for existence of files
 I/O:
    input parameters: String and Const Char
    output: boolean.
 Note: Function overloading
 *************************************************************/
bool fileExists(string filename);
bool fileExists(const char* filename);

/*************************************************************
 FUNCTION: createFile()
 DESCRIPTION: Creates a new file
 I/O:
    input parameters: String
    output: Pointer to a created file.
 *************************************************************/
FILE* createFile(string filename);

/*************************************************************
 FUNCTION: closeFile()
 DESCRIPTION: Closes an already open file
 I/O:
    input parameters: Pointer to a  file.
    output: Boolean
 *************************************************************/
bool closeFile(FILE* fp);

/*************************************************************
 FUNCTION: hexToDecimal()
 DESCRIPTION: Converts hex to decimal
 I/O:
    input parameters: String
    output: Integer
 *************************************************************/
int hexToDecimal(string num);

/*************************************************************
 FUNCTION: signedHexToDecimal()
 DESCRIPTION: Converts signed hex to decimal
 I/O:
    input parameters: String
    output: Integer
 *************************************************************/
int signedHexToDecimal(string num);

/*************************************************************
 FUNCTION: decimalToHex()
 DESCRIPTION: Converts decimal to hex
 I/O:
    input parameters: Integer
    output: String
 *************************************************************/
string decimalToHex(int num);

/*************************************************************
 FUNCTION: toString(int num)
 DESCRIPTION: Converts an integer to its string representation
 I/O:
    input parameters: Integer
    output: String.
 *************************************************************/
string toString(int num);

/*************************************************************
 FUNCTION: nixbpeFinder(string hex)
 DESCRIPTION: checks three nibbles for nixbpe bits
 I/O:
    input parameters: String
    output: String
 *************************************************************/
string nixbpeFinder(string hex);

/*************************************************************
 FUNCTION: opcodeValid()
 DESCRIPTION: Checks if the given opcode is valid
 I/O:
    input parameters: String
    output: Boolean
 *************************************************************/
bool opcodeValid(string hex);

/*************************************************************
 FUNCTION: formatFinder()
 DESCRIPTION: Finds format of an instruction based off its 
 opcode.
 I/O:
    input parameters: String
    output: Integer
 *************************************************************/
int formatFinder(string hex);

/*************************************************************
 FUNCTION: hexToCommand()
 DESCRIPTION: Converts hex opcode to respective function
 I/O:
    input parameters: String
    output: String.
 *************************************************************/
string hexToCommand(string hex);

/*************************************************************
 CLASS: Symbol
 DESCRIPTION: Internal datastructure to store symbols and its
 properties from Symtab
 *************************************************************/
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


/*************************************************************
 CLASS: Literal
 DESCRIPTION: Internal datastructure to store literals and its
 properties from Littab.
 *************************************************************/
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

/*************************************************************
 FUNCTION: findAddressInSymtab(Symbol *symPtr, string address)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to sym linked list, checking address(6 char string)
    output: Symbol pointer
 NOTE: only designed to work with 6 character strings for address arg.
 *************************************************************/
Symbol* findAddressInSymtab(Symbol *symPtr, string address);

/*************************************************************
 FUNCTION: findAddressInLittab(Symbol *symPtr, string address)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to sym linked list, checking address(6 char string)
    output: Symbol pointer
 NOTE: only designed to work with 6 character strings for address arg.
 *************************************************************/
Literal* findAddressInLittab(Literal *litPtr, string address);

/*************************************************************
 FUNCTION: toSymbol(Symbol *head, FILE *fp)
 DESCRIPTION: Reads the .sym file and transfers the information onto a linked list.
 I/O:
    Input: Pointer to head of symbol linked list. File pointer to Littab.
    Output: Returns pointer to the new head of the symbol linked list.
 *************************************************************/
Symbol* toSymbol(Symbol* head, FILE *fp);

/*************************************************************
 FUNCTION: toLiteral(Literal* &head, FILE *fp)
 DESCRIPTION: Reads the .sym file and transfers the information onto a linked list.
 I/O:
    Input: Pointer to head of literal linked list. File pointer to Littab.
    Output: Returns pointer to the new head of the literal linked list.
 *************************************************************/
Literal* toLiteral(Literal* &head, FILE *fp);

/*************************************************************
 FUNCTION: readObj(FILE *fp)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to FILE struct
    output: Vector<string>
 *************************************************************/
vector<string> readObj(FILE *fp, Symbol *symHead, Literal *litHead);

/*************************************************************
 FUNCTION: concatTrailingSpaces(string s)
 DESCRIPTION: Concatinates trailing spaces
 I/O:
    input parameters: String
    output: String
 *************************************************************/
 string concatTrailingSpaces(string s);

/*************************************************************
 FUNCTION: writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead)
 DESCRIPTION: writes the sic file based on the info provided
 I/O:
    input parameters: FILE pointer, vector<string>, Symbol pointer, Literal pointer
    output: n/a
 *************************************************************/
void writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead);

/*************************************************************
 FUNCTION: writeAddress(FILE *fp, int address)
 DESCRIPTION: converts decimal address to hex address for the lis file
 I/O:
    input parameters: FILE pointer, int address
    output: n/a
 *************************************************************/
void writeAddress(FILE *fp, int address);

/*************************************************************
 FUNCTION: writeAddress(FILE *fp, int address)
 DESCRIPTION: converts decimal address to hex address for the lis file
 I/O:
    input parameters: FILE pointer, int address
    output: n/a
 *************************************************************/
void writeOpcode(FILE *fp, string opcode, int column);

/*************************************************************
 FUNCTION: writeLisFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead)
 DESCRIPTION: writes the lis file based on the info provided
 I/O:
    input parameters: FILE pointer, vector<string>, Symbol pointer, Literal pointer
    output: n/a
 *************************************************************/
void writeLisFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead);

/**************************[ EOF: xbd.hpp ]************************/