/*************************************************************
 Name: Blake Meyers(cssc0430), Darpan Beri(cssc0429), Zach Selchau(cssc0418)
 Project: CS530 Assignment 2
 File: xed.cpp
 Notes: The file cointains an algorithm that opens an XE object
 file and its accompanying symbol file, which then disassembles
 the object code, and generate an XE source file, and XE
 listing file using the disassembled code.
 *************************************************************/

 /*
    Zach and DB added toSymbol and toLiteral and linked list datastructure to traverse through symbols and literals(different linked list respectively).
    Note: The first symbol is the last element of the linked list and so on.
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <exception>
#include <cstring>
#include <cstdio>
#include <sstream>

using namespace std;
/*
    Pseduo code as of now:
        1. We check for correct input:
            a. Check for actual argument
            b. check for existence for .obj and .sym file
        1.b If check fail, do graceful exit
            - obj file not found
            - sym file not found
            - Both obj and sym file not found
            - No argument given

        2. Initalize .sic and .lis files. Maybe make one more intermediatory file for debugging purposes.
            a. Create file via C++.

        INTERNAL DATASTRUCTURE:
            1.Symbols: Has a symbolName that has a value and flag associated with it
            2.Literals: have length and address

        3. Algorithm for disassembling
            ---- NEED INTERNAL DATA STRUCTURE FOR STORING .SYM FILE----------
            a. Read the .obj and .sym file.
            b. Interpret the contents via code.
            c. Write line by line? or store in intermediatory file and then write to .lis and .sic.

        3.5  Writing it to files
            d. Write the .lis file
            e. Write the .sic file
*/

/* INPUT AND OUTPUT STRUCTURE:
    Input                   Output
    <filename>.obj           <filename>.sic
    <filename>.sym           <filename>.lis

    % xed <filename>

*/

/*  Tips from prof:

    Graceful Exit:
        exit() -> Test return value on every system call. Int error handling routine

    Don't use templates in C++.
    Use C versions of I/O commands
*/

/*
    NEW LINE FEED: '0A', Used Hex editor to find that Out
    Need to add 0A to output files too.
*/

/*************************************************************
 FUNCTION: gracefulExit()
 DESCRIPTION: Exits gracefully
 I/O:
    input parameters: String
    output: Void.
 *************************************************************/
void gracefulExit(string msg){
    cout << msg << endl;
    exit(EXIT_FAILURE);
}

/*************************************************************
 FUNCTION: fileExists()
 DESCRIPTION: Check for existence of files
 I/O:
    input parameters: String and Const Char
    output: boolean.
 Note: Function overloading
 *************************************************************/
bool fileExists(string filename){
    FILE* fp;
    if(fp = fopen(filename.c_str(), "r")){
        fclose(fp);
        return true;
    }
    return false;
}
bool fileExists(const char* filename){
    FILE* fp;
    if(fp = fopen(filename, "r")){
        fclose(fp);
        return true;
    }
    return false;
}

/*************************************************************
 FUNCTION: createFile()
 DESCRIPTION: Creates a new file
 I/O:
    input parameters: String
    output: Pointer to a created file.
 *************************************************************/
FILE* createFile(string filename){
    FILE* fp;

    fp = fopen(filename.c_str(), "w");

    if(!fp)gracefulExit("Fatal Error: failed to create file '"+filename+"'");

    return fp;
}

bool closeFile(FILE* fp){
    return fclose(fp) == 0;
}

/*************************************************************
 FUNCTION: hexToDecimal()
 DESCRIPTION: Converts hex to Decimal
 I/O:
    input parameters: String
    output: Integer
 *************************************************************/
int hexToDecimal(string num){
    int x;
    stringstream ss;

    ss << hex << "0"+num;
    ss >> x;

    return x;
}

//Returns a 6 char string
/*************************************************************
 FUNCTION: hexToCommand()
 DESCRIPTION: Converts hex opcode to respective function
 I/O:
    input parameters: String
    output: String.
 *************************************************************/
string hexToCommand(string hex){
    int x = hexToDecimal(hex);
    x /= 4;

    //Produces an error on 8C, CC, and E4
    switch(x){//command library
        case 0://00
            return "LDA   ";
            break;
        case 1://04
            return "LDX   ";
            break;
        case 2://08
            return "LDL   ";
            break;
        case 3://0C
            return "STA   ";
            break;
        case 4://10
            return "STX   ";
            break;
        case 5://14
            return "STL   ";
            break;
        case 6://18
            return "ADD   ";
            break;
        case 7://1C
            return "SUB   ";
            break;
        case 8://20
            return "MUL   ";
            break;
        case 9://24
            return "DIV   ";
            break;
        case 10://28
            return "COMP  ";
            break;
        case 11://2C
            return "TIX   ";
            break;
        case 12://30
            return "JEQ   ";
            break;
        case 13://34
            return "JGT   ";
            break;
        case 14://38
            return "JLT   ";
            break;
        case 15://3C
            return "J     ";
            break;
        case 16://40
            return "AND   ";
            break;
        case 17://44
            return "OR    ";
            break;
        case 18://48
            return "JSUB  ";
            break;
        case 19://4C
            return "RSUB  ";
            break;
        case 20://50
            return "LDCH  ";
            break;
        case 21://54
            return "STCH  ";
            break;
        case 22://58
            return "ADDF  ";
            break;
        case 23://5C
            return "SUBF  ";
            break;
        case 24://60
            return "MULF  ";
            break;
        case 25://64
            return "DIVF  ";
            break;
        case 26://68
            return "LDB   ";
            break;
        case 27://6C
            return "LDS   ";
            break;
        case 28://70
            return "LDF   ";
            break;
        case 29://74
            return "LDT   ";
            break;
        case 30://78
            return "STB   ";
            break;
        case 31://7C
            return "STS   ";
            break;
        case 32://80
            return "STF   ";
            break;
        case 33://84
            return "STT   ";
            break;
        case 34://88
            return "COMPF ";
            break;
        case 35://8C
            return "ERROR ";
            break;
        case 36://90
            return "ADDR  ";
            break;
        case 37://94
            return "SUBR  ";
            break;
        case 38://98
            return "MULR  ";
            break;
        case 39://9C
            return "DIVR  ";
            break;
        case 40://A0
            return "COMPR ";
            break;
        case 41://A4
            return "SHIFTL";
            break;
        case 42://A8
            return "SHIFTR";
            break;
        case 43://AC
            return "RMO   ";
            break;
        case 44://B0
            return "SVC   ";
            break;
        case 45://B4
            return "CLEAR ";
            break;
        case 46://B8
            return "TIXR  ";
            break;
        case 47://BC
            return "SUBR  ";
            break;
        case 48://C0
            return "FLOAT ";
            break;
        case 49://C4
            return "FIX   ";
            break;
        case 50://C8
            return "OR    ";
            break;
        case 51://CC
            return "ERROR ";
            break;
        case 52://D0
            return "LPS   ";
            break;
        case 53://D4
            return "STI   ";
            break;
        case 54://D8
            return "RD    ";
            break;
        case 55://DC
            return "WD    ";
            break;
        case 56://E0
            return "TD    ";
            break;
        case 57://E4
            return "ERROR ";
            break;
        case 58://E8
            return "STSW  ";
            break;
        case 59://EC
            return "SSK   ";
            break;
        case 60://F0
            return "SIO   ";
            break;
        case 61://F4
            return "HIO   ";
            break;
        case 62://F8
            return "TIO   ";
            break;
    }

    return "ERROR";
}

class Symbol{
    private:
        string name;
        char flag;
        string value;
        int decValue;

    public:
        Symbol *next;

        Symbol(string nam, string val, char flg, Symbol *nextSym){
            this->name = nam;
            this->flag = flg;
            this->value = val;
            this->decValue = hexToDecimal(val);
            this->next = nextSym;
            //cout << this->name << ", " << this->value << ", " << this->flag << endl;
        }

        char getFlag(){
            return this->flag;
        }

        string getValue(){
            return this->value;
        }

        int getDecValue(){
            return this->decValue;
        }

        void setNext(Symbol *nextSym){
            this->next = nextSym;
        }

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

        Literal(string nam, string addy, string len, Literal *nextLit){
            this->name = nam;
            this->address = addy;
            this->length = len;
            this->decAddress = hexToDecimal(addy);
            this->decLength = hexToDecimal(len);
            this->next = nextLit;
            //cout << this->name << ", " << this->address << ", " << this->length << endl;
        }

        string getAddress(){
            return this->address;
        }

        string getLength(){
            return this->length;
        }

        int getDecAddress(){
            return this-> decAddress;
        }

        int getDecLength(){
            return this-> decLength;
        }

        void setNext(Literal *nextLit){
            this->next = nextLit;
        }
};

//Reading symtab
    /**
        1. iterate past first two '0a'
        2. save the next six bytes as a string 'name' for symbol
        3. skip 2 bytes (spaces)
        4. take next six bytes as values
        5. skip 2 more bytes (spaces)
        6. take next byte as a flag
        7. check for two 'oa' bytes in a row, else jump 2
        8. skip until found two more 'oa'
        9. skip all whitespace
        10. read until whitespace, save into literal name
        11. skip whitespace until not whitespace
        12. take bytes into length until whitespace
        13. skip whitespace until not whitespace
        14. take in 6 bytes into address
        15. go until another '0a' and test if you can read one more byte
    **/

Symbol* toSymbol(Symbol* head, FILE *fp){
    int c = 0;
    string tmpName = "";
    string tmpValue = "";
    char tmpFlag;

    if(feof(fp))return head;
    for(int i = 0; i<2; i++){//1. iterate past first two '0a'
        c = fgetc(fp);
        if(c != 10) i--;
    }

    while(!feof(fp)){// Started taking in Symbols
        for(int i = 0; i<6; i++){//2. save the next six bytes as a string 'name' for symbol
            c =fgetc(fp);
            if(i==0 && c == 10)return head;//7. check for two 'oa' bytes in a row, else jump 2
            char s = static_cast<char>(c);
            tmpName += s;
        }

        c =fgetc(fp);//3. skip 2 bytes (spaces)
        c =fgetc(fp);

        for(int i = 0; i<6; i++){//4. take next six bytes as values
            c =fgetc(fp);
            char s = static_cast<char>(c);
            tmpValue += s;
        }

        c =fgetc(fp);//5. skip 2 bytes (spaces)
        c =fgetc(fp);

        c = fgetc(fp);//6. take next byte as a flag
        tmpFlag = static_cast<char>(c);

        Symbol *tmpSym = new Symbol(tmpName, tmpValue, tmpFlag, head);//6.5 create Symbol object and put in linked list backwards
        head = tmpSym;
        cout << tmpSym->next <<endl;
        tmpName = "";
        tmpValue = "";

        c = fgetc(fp);//7. check for two 'oa' bytes in a row, else jump 2
    }

    return head;
}

/*
    8. skip until found two more 'oa'
    9. skip all whitespace
        10. read until whitespace, save into literal name
        11. skip whitespace until not whitespace
        12. take bytes into length until whitespace
        13. skip whitespace until not whitespace
        14. take in 6 bytes into address
        15. go until another '0a' and test if you can read one more byte
*/

Literal* toLiteral(Literal* &head, FILE *fp){
    int c = 0;
    string tmpName = "";
    string tmpAddr = "";
    string tmpLen = "";

    if(feof(fp))return head;
    for(int i = 0; i<2; i++){// 8. skip until found two more '0a'
         c = fgetc(fp);
        if(c != 10) i--;
    }

    while(!feof(fp)){ // either EOF or whitespace
        c = fgetc(fp);

        if(c == -1)return head;

        while(c == 32)c = fgetc(fp);

        while(c != 32){ // 12. take bytes into name until whitespace
            char s = static_cast<char>(c);
            tmpName += s;
            c = fgetc(fp);
        }

        while(c == 32)c = fgetc(fp); // 13. skip whitespace until not whitespace

        // save to temp length
        while(c != 32){ // 12. take bytes into length until whitespace
            char s = static_cast<char>(c);
            tmpLen += s;
            c = fgetc(fp);
        }

        while(c == 32)c = fgetc(fp); // 13. skip whitespace until not whitespace

        for(int i = 0; i<6; i++){// 14. take in 6 bytes into address
            char s = static_cast<char>(c);
            tmpAddr += s;
            c =fgetc(fp);
        }

        // Save to literal
        Literal *tmpLit = new Literal(tmpName, tmpAddr, tmpLen, head);//6.5 create Literal object and put in linked list backwards
        head = tmpLit;
        tmpName = "";
        tmpAddr = "";
        tmpLen = "";


        while (c==32)c = fgetc(fp); // c becomes '0A' && 15. go until another '0a' and test if you can read one more byte

    }

    return head;
}

int main(int argc, char* argv[]){
    //Case 1: there are no arguments passed
    if(argc != 2)gracefulExit("Fatal Error: no filename given.");//exit()

    //Case 2: obj or sym file dne
    string objFile (argv[1]);
    string symFile = objFile + ".sym";
    objFile = objFile + ".obj";
    if(!fileExists(objFile))gracefulExit("Fatal Error: object file not found.");//exit()
    if(!fileExists(symFile))gracefulExit("Fatal Error: symbol file not found.");//exit()

    Symbol *symHead = nullptr;//Create sym head and fp
    Literal *litHead = nullptr;
    FILE *fp = fopen(symFile.c_str(), "r");
    symHead = toSymbol(symHead, fp);//pass to toSymbol
                            //check if next byte exists w/ while(!feof(fp)){}
    litHead = toLiteral(litHead, fp);

    cout << symHead->getValue() << endl;
    cout << symHead->next->getValue() << endl;
    cout << litHead->getAddress() << endl;

}
