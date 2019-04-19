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
#include <vector>
#include <algorithm>

#define nullptr NULL

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
 DESCRIPTION: Converts hex to decimal
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

/*************************************************************
 FUNCTION: signedHexToDecimal()
 DESCRIPTION: Converts signed hex to decimal
 I/O:
    input parameters: String
    output: Integer
 *************************************************************/
int signedHexToDecimal(string num){
    string leadF = "";

    if(num.substr(0,1) == "8" || num.substr(0,1) == "9" || num.substr(0,1) == "A" ||
        num.substr(0,1) == "B" || num.substr(0,1) == "C" || num.substr(0,1) == "D" ||
        num.substr(0,1) == "E" || num.substr(0,1) == "F")
            for(int i = num.length(); i < 9; i++) leadF += "F";

    leadF += num;
    
    int x = (int)strtol(leadF.c_str(), 0, 16);

    return x;
}

/*************************************************************
 FUNCTION: decimalToHex()
 DESCRIPTION: Converts decimal to hex
 I/O:
    input parameters: int num
    output: string hex
 *************************************************************/
string decimalToHex(int num){
    stringstream ss;

    ss << hex << num;
    
    string s (ss.str());

    transform(s.begin(), s.end(), s.begin(), ::toupper);        

    while(s.length() < 6) s = "0"+s;
    if(s.length() > 6) s = s.substr(s.length()-6,6);
    return s;
}

/*************************************************************
 FUNCTION: nixbpeFinder(string hex)
 DESCRIPTION: checks three nibbles for nixbpe bits
 I/O:
    input parameters: three hex characters
    output: string.
 *************************************************************/
 string toString(int num){
	stringstream ss;
	ss << num;
	return ss.str();
 }

/*************************************************************
 FUNCTION: nixbpeFinder(string hex)
 DESCRIPTION: checks three nibbles for nixbpe bits
 I/O:
    input parameters: three hex characters
    output: string.
 *************************************************************/
string nixbpeFinder(string hex){
    int x = hexToDecimal(hex.substr(0,2));//First two digits
    int y = hexToDecimal(hex.substr(2,1));//Last digit
    string tmpStr = "";


    x %= 4;
    if(x==3)tmpStr += "11";
    if(x==2)tmpStr += "10";
    if(x==1)tmpStr += "01";


    if(y>=8){
        y-=8;
        tmpStr += "1";
    }
    else tmpStr += "0";
    if(y>=4){
        y-=4;
        tmpStr += "1";
    }
    else tmpStr += "0";
    if(y>=2){
        y-=2;
        tmpStr += "1";
    }
    else tmpStr += "0";
    if(y>=1){
        y-=1;
        tmpStr += "1";
    }
    else tmpStr += "0";


    return tmpStr;
}

/*************************************************************
 FUNCTION: opcodeValid()
 DESCRIPTION: Checks if the given opcode is valid
 I/O:
    input parameters: hex String
    output: boolean
 *************************************************************/
bool opcodeValid(string hex){
    
    string nixbpe = nixbpeFinder(hex);//Note-- only works on 3 hex digits
    
    if(nixbpe.substr(0,2)=="00") return false;
    if(nixbpe.substr(0,1)=="0" || nixbpe.substr(1,1)=="0")
        if(nixbpe.substr(2,1)=="1")return false;
    if(nixbpe.substr(3,1)=="1" && (nixbpe.substr(4,1)=="1" || nixbpe.substr(5,1)=="1")) return false;
    if(nixbpe.substr(4,1)=="1" && nixbpe.substr(5,1)=="1") return false;
    
    
    
    return true;
}


/*************************************************************
 FUNCTION: formatFinder()
 DESCRIPTION: Finds format of an instruction based off its 
 opcode.
 I/O:
    input parameters: String
    output: int.
 *************************************************************/
int formatFinder(string hex){
    int x = hexToDecimal(hex);
    x /= 4;

    //Produces an error on 8C, CC, and E4
    switch(x){//command library
        case 0://00
            return 3;
            break;
        case 1://04
            return 3;
            break;
        case 2://08
            return 3;
            break;
        case 3://0C
            return 3;
            break;
        case 4://10
            return 3;
            break;
        case 5://14
            return 3;
            break;
        case 6://18
            return 3;
            break;
        case 7://1C
            return 3;
            break;
        case 8://20
            return 3;
            break;
        case 9://24
            return 3;
            break;
        case 10://28
            return 3;
            break;
        case 11://2C
            return 3;
            break;
        case 12://30
            return 3;
            break;
        case 13://34
            return 3;
            break;
        case 14://38
            return 3;
            break;
        case 15://3C
            return 3;
            break;
        case 16://40
            return 3;
            break;
        case 17://44
            return 3;
            break;
        case 18://48
            return 3;
            break;
        case 19://4C
            return 3;
            break;
        case 20://50
            return 3;
            break;
        case 21://54
            return 3;
            break;
        case 22://58
            return 3;
            break;
        case 23://5C
            return 3;
            break;
        case 24://60
            return 3;
            break;
        case 25://64
            return 3;
            break;
        case 26://68
            return 3;
            break;
        case 27://6C
            return 3;
            break;
        case 28://70
            return 3;
            break;
        case 29://74
            return 3;
            break;
        case 30://78
            return 3;
            break;
        case 31://7C
            return 3;
            break;
        case 32://80
            return 3;
            break;
        case 33://84
            return 3;
            break;
        case 34://88
            return 3;
            break;
        case 35://8C
            return 0;
            break;
        case 36://90
            return 2;
            break;
        case 37://94
            return 2;
            break;
        case 38://98
            return 2;
            break;
        case 39://9C
            return 2;
            break;
        case 40://A0
            return 2;
            break;
        case 41://A4
            return 2;
            break;
        case 42://A8
            return 2;
            break;
        case 43://AC
            return 2;
            break;
        case 44://B0
            return 2;
            break;
        case 45://B4
            return 2;
            break;
        case 46://B8
            return 2;
            break;
        case 47://BC
            return 0;
            break;
        case 48://C0
            return 1;
            break;
        case 49://C4
            return 1;
            break;
        case 50://C8
            return 1;
            break;
        case 51://CC
            return 0;
            break;
        case 52://D0
            return 3;
            break;
        case 53://D4
            return 3;
            break;
        case 54://D8
            return 3;
            break;
        case 55://DC
            return 3;
            break;
        case 56://E0
            return 3;
            break;
        case 57://E4
            return 0;
            break;
        case 58://E8
            return 3;
            break;
        case 59://EC
            return 3;
            break;
        case 60://F0
            return 1;
            break;
        case 61://F4
            return 1;
            break;
        case 62://F8
            return 1;
            break;
    }

    return 0;
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
            return "ERROR ";
            break;
        case 48://C0
            return "FLOAT ";
            break;
        case 49://C4
            return "FIX   ";
            break;
        case 50://C8
            return "NORM  ";
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
        
        string getName(){
            return this->name;
        }

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

        string getName(){
            return this->name;
        }
};

/*************************************************************
 FUNCTION: addressInSymtab(Symbol *symPtr, string address)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to sym linked list, checking address(6 char string)
    output: symbol pointer
 NOTE: only designed to work with 6 character strings for address arg.
 *************************************************************/
Symbol* findAddressInSymtab(Symbol *symPtr, string address){
    while(symPtr!=nullptr){
        if(symPtr->getValue() == address)return symPtr;
        symPtr = symPtr->next;
    }
    return symPtr;
}

/*************************************************************
 FUNCTION: addressInLittab(Symbol *symPtr, string address)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to sym linked list, checking address(6 char string)
    output: symbol pointer
 NOTE: only designed to work with 6 character strings for address arg.
 *************************************************************/
Literal* findAddressInLittab(Literal *litPtr, string address){
    while(litPtr!=nullptr){
        if(litPtr->getAddress() == address)return litPtr;
        litPtr = litPtr->next;
    }
    return litPtr;
}

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

/*************************************************************
 FUNCTION: toSymbol(Symbol *head, FILE *fp)
 DESCRIPTION: Reads the .sym file and transfers the information onto a linked list.
 I/O:
    Input: Pointer to head of symbol linked list. File pointer to Littab.
    Output: Returns pointer to the new head of the symbol linked list.
 *************************************************************/
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

        Symbol *tmpSym = new Symbol(tmpName, tmpValue, tmpFlag, nullptr);//6.5 create Symbol object
        if(head == nullptr) head = tmpSym;
        else{
            Symbol *symPtr = head;
            while(symPtr->next !=nullptr) symPtr = symPtr->next;
            symPtr->next = tmpSym;
        }
        //cout << tmpSym->next <<endl;
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
/*************************************************************
 FUNCTION: toLiteral(Literal* &head, FILE *fp)
 DESCRIPTION: Reads the .sym file and transfers the information onto a linked list.
 I/O:
    Input: Pointer to head of literal linked list. File pointer to Littab.
    Output: Returns pointer to the new head of the literal linked list.
 *************************************************************/
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
        Literal *tmpLit = new Literal(tmpName, tmpAddr, tmpLen, nullptr);//6.5 create Literal object
        if(head == nullptr) head = tmpLit;
        else{
            Literal *litPtr = head;
            while(litPtr->next !=nullptr) litPtr = litPtr->next;
            litPtr->next = tmpLit;
        }
        head = tmpLit;
        tmpName = "";
        tmpAddr = "";
        tmpLen = "";


        while (c==32)c = fgetc(fp); // c becomes '0A' && 15. go until another '0a' and test if you can read one more byte

    }

    return head;
}

/*************************************************************
 FUNCTION: readObj(FILE *fp)
 DESCRIPTION: Reads the object file and transfers the information onto a vector<string>
 I/O:
    input parameters: Pointer to FILE struct
    output: Vector<string>
 *************************************************************/
vector<string> readObj(FILE *fp, Symbol *symHead, Literal *litHead){
    
    vector<string> tmpVector;
    string tmpVar = "";
    int tmpFormat = 0;
    int address = 0;

    int c = fgetc(fp);
    if(c != 72)gracefulExit("Fatal Error: no header record found in object file."); // No header record found


    tmpVector.push_back("H");
    for(int i=0; i<18; i++){ // Storing Header record in tmpVar
        c= fgetc(fp);// At first character of the name
        char s = static_cast<char>(c);
        tmpVar += s;
    }

    address = hexToDecimal(tmpVar.substr(6,6));//Initialize address to the start of program

    tmpVector.push_back(tmpVar); // Header in 0th spot
    tmpVar = "";

    while(c!=10)c=fgetc(fp); // Just in case
    c = fgetc(fp);
    

    tmpVector.push_back("T");
    while(c == 84){ // While in Text Record

        for(int i=0;i<9;i++)c=fgetc(fp); // skips first 9 characters in that line.

        
        while(c!=10){// First opcode instruction
            
            if(litHead!=nullptr && litHead->getDecAddress() <= address){//Literal (LTORG)
                for(int i=0; i < litHead->getDecLength();i++){
                    char s = static_cast<char>(c);
                    tmpVar += s;
                    c=fgetc(fp);
                }
            
                tmpVector.push_back(tmpVar);
                tmpVar="";
                address += litHead->getDecLength()/2;
                litHead = litHead->next;
                continue;
            }
            
            for(int i=0;i<2;i++){ // First 2 characters of first opcode in tmpVar to check format
                char s = static_cast<char>(c);
                tmpVar += s;
                c=fgetc(fp);
            }

            tmpFormat = formatFinder(tmpVar);
        
        
            if(tmpFormat == 3){//Format 3/4
                int j = 3;

                char s = static_cast<char>(c);//Check next nibble
                tmpVar += s;
                c=fgetc(fp);

                if(!opcodeValid(tmpVar)){//Check for invalid nixbpe
                    /*
                        if invalid then this has to be a WORD
                    */
                }
                else if(s == '1'||s == '3'||s == '5'||s == '7'||s == '9'||s == 'B'||s == 'D'||s == 'F'){//Format 4
                    j=5;
                    address+=1;
                }
            
                for(int i=0; i<j; i++){//Store given number of nibbles
                    s= static_cast<char>(c);
                    tmpVar += s;
                    c=fgetc(fp);
                }
                tmpVector.push_back(tmpVar);//Storing in opcode
                tmpVar = "";
                address+=3;
            }
            else if(tmpFormat == 2){//Format 2
                char s = static_cast<char>(c);//Need one more byte
                tmpVar += s;
                c=fgetc(fp);

                s = static_cast<char>(c);
                tmpVar += s;
                c=fgetc(fp);

                tmpVector.push_back(tmpVar);//Storing in opcode
                tmpVar = "";
                address+=2;
            }
            else if(tmpFormat == 1){//Format 1
                tmpVector.push_back(tmpVar);//Storing in opcode
                tmpVar = "";
                address+=1;
            }
        }
        c=fgetc(fp);
    }


    tmpVector.push_back("M");
    while(c==77){//Modification record
        c= fgetc(fp);
        while(c!=10){
            char s = static_cast<char>(c);
            tmpVar += s;
            c= fgetc(fp);
        }
        tmpVector.push_back(tmpVar);
        tmpVar = "";
        c= fgetc(fp);
    }

    //End record check
    if(c!=69) gracefulExit("Fatal Error: no end record found.");

    tmpVector.push_back("E");//69
    for(int i=0; i<6; i++){
        c= fgetc(fp);
        char s = static_cast<char>(c);
        tmpVar += s;
    }

    tmpVector.push_back(tmpVar);
    tmpVar = "";

    return tmpVector;
}

/*************************************************************
 FUNCTION: writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead)
 DESCRIPTION: writes the sic file based on the info provided
 I/O:
    input parameters: FILE pointer, vector<string>, Symbol pointer, Literal pointer
    output: n/a
 *************************************************************/
 string concatTrailingSpaces(string s){
     
     while(s.substr(s.length()-1,1) == " ")s = s.substr(0, s.length()-1);

     return s;
 }

/*************************************************************
 FUNCTION: writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead)
 DESCRIPTION: writes the sic file based on the info provided
 I/O:
    input parameters: FILE pointer, vector<string>, Symbol pointer, Literal pointer
    output: n/a
 *************************************************************/
void writeSicFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead){
    //Instets 1st line of SIC program
    fprintf(fp, "%s", objVector[1].substr(0,6).c_str());
    fprintf(fp, "   START   ");
    fprintf(fp, "%s", objVector[1].substr(6,6).c_str());
    fputc(10, fp);
     
    //Text records
    int index = 3;
    int address = hexToDecimal(objVector[1].substr(6,6));;
    int baseAddress = 0;
    Symbol *symPtr = symHead; // Potential optimization
    Literal *litPtr = litHead; // Potential optimization
    string nixbpeStr = "";
    while(objVector[index] != "M"){

        //First 8 columns(1 based)
        if(symPtr != nullptr && symPtr->getDecValue() == address){
            fprintf(fp, "%s  ", symPtr->getName().c_str());
            symPtr = symPtr->next;
        }
        else fprintf(fp, "        ");

        //Column 9(1 based)
        if(formatFinder(objVector[index].substr(0,2))==3 && opcodeValid(objVector[index].substr(0,3))){
            nixbpeStr = nixbpeFinder(objVector[index].substr(0,3));
            if(nixbpeStr.substr(5,1) == "1") fprintf(fp, "+");//extended
            else fprintf(fp," ");
        }else fprintf(fp, " ");

        //Columns 10-16(1 based)
        if(litPtr!=nullptr && litPtr->getDecAddress()==address){
            fprintf(fp, "LTORG  ");
            fputc(10, fp);
            address += objVector[index++].length()/2;
            litPtr = litPtr->next;
            continue;
        }
        fprintf(fp, "%s ", hexToCommand(objVector[index].substr(0,2)).c_str()); // General case

        //Column 17
        if(formatFinder(objVector[index].substr(0,2))==3){
            nixbpeStr = nixbpeFinder(objVector[index].substr(0,3));
            if(nixbpeStr.substr(0,1)=="0") fputc(35, fp);
            else if(nixbpeStr.substr(1,1)=="0") fputc(64, fp);
            else fputc(32, fp);
        }

        //Base Register checker
        if(hexToCommand(objVector[index].substr(0,2))=="LDB   "){
            int checkNibbles = 3;
            int checkAddress = 0;
            if(nixbpeStr.substr(5,1)=="1") checkNibbles = 5;
            else if(nixbpeStr.substr(4,1)=="1") checkAddress = address;
            else if(nixbpeStr.substr(3,1)=="1") checkAddress = baseAddress;

            if(nixbpeStr.substr(1,1)=="1") baseAddress = checkAddress + hexToDecimal(objVector[index].substr(3,checkNibbles));
            
        }

        //Columns 18-35
        /*
            We have to determine the address from the symtable. We know if its a direct address, immediate address, base relative or pc relative etc.
            We dont know if exactly where the BASE assembler directive would be. If LDB is every given, we can very well assume that the very next instruction is going to be to base relative.
            Take opcode and check the format.
            Depending of the format, the structure will change:
                * If Format 3 and 4 a lot of bs
                * If format 2, interpret the next 2 nibbles  of the opcode and put the corresponding register to the correct spots and add in 0A.
                * Format 1, do nothing and add 0A.
        */
        if(objVector[index].substr(0,2)=="4F")fputc(10, fp);
        else if(formatFinder(objVector[index].substr(0,2))==3){ // Format 3/4
            if(nixbpeStr.substr(5,1)=="0"){ // if not extended
                int tmpAddress = -1; // placeholder
                
                if(nixbpeStr.substr(4,1)=="1") tmpAddress = address + 3 + signedHexToDecimal(objVector[index].substr(3,3)); // pc relative
                else if(nixbpeStr.substr(3,1)=="1") tmpAddress = baseAddress + hexToDecimal(objVector[index].substr(3,3)); // base relative
                else tmpAddress = hexToDecimal(objVector[index].substr(3,3)); // nether base nor pc realtive
                
                Symbol *tmpSymPtr = findAddressInSymtab(symHead, decimalToHex(tmpAddress)); // check if in symtab
                Literal *tmpLitPtr = findAddressInLittab(litHead, decimalToHex(tmpAddress-3)); // check if in littab
                
                if(tmpSymPtr != nullptr){
                    string s = concatTrailingSpaces(tmpSymPtr->getName());
                    fprintf(fp, "%s", s.c_str()); // if in symtab print out symbol name
                }
                else if(tmpLitPtr != nullptr){
                    string s = concatTrailingSpaces(tmpLitPtr->getName());
                    fprintf(fp, "%s", s.c_str()); // else if in littab, print litname
                }
                else {
                    fprintf(fp, "%s", objVector[index].substr(3,3).c_str()); // else print remaining info
                }
     
            }
            else { // if extended i.e. not relative
                Symbol *tmpSymPtr = findAddressInSymtab(symHead, "0"+objVector[index].substr(3,5)); // check in symtab
                Literal *tmpLitPtr = findAddressInLittab(litHead, "0"+objVector[index].substr(3,5)); // check if in littab

                if(tmpSymPtr != nullptr){
                    string s = concatTrailingSpaces(tmpSymPtr->getName());
                    fprintf(fp, "%s", s.c_str()); // if in symtab print out symbol name
                }
                else if(tmpLitPtr != nullptr){
                    string s = concatTrailingSpaces(tmpLitPtr->getName());
                    fprintf(fp, "%s", s.c_str()); // else if in littab, print litname
                }
                else fprintf(fp, "%s", objVector[index].substr(3,5).c_str()); // else print remaining info
            }
            

            if(nixbpeStr.substr(2,1)=="1") fprintf(fp, ",X"); // check if indexed

            fputc(10, fp);
        }
        else if(formatFinder(objVector[index].substr(0,2))==2){//Format 2
            int x = hexToDecimal(objVector[index].substr(2,1));
            switch(x){
                case 0:
                    fprintf(fp, "A");
                    break;
                case 1:
                    fprintf(fp, "X");
                    break;
                case 2:
                    fprintf(fp, "L");
                    break;
                case 3:
                    fprintf(fp, "B");
                    break;
                case 4:
                    fprintf(fp, "S");
                    break;
                case 5:
                    fprintf(fp, "T");
                    break;
                case 6:
                    fprintf(fp, "F");
                    break;
                case 8:
                    fprintf(fp, "PC");
                    break;
                case 9:
                    fprintf(fp, "SW");
                    break;
            }
            if(objVector[index].substr(0,2)!="B0"&&objVector[index].substr(0,2)!="B4"&&objVector[index].substr(0,2)!="B8"){ // check if second argument
                x = hexToDecimal(objVector[index].substr(3,1));
                switch(x){
                    case 0:
                        fprintf(fp, ",A");
                        break;
                    case 1:
                        fprintf(fp, ",X");
                        break;
                    case 2:
                        fprintf(fp, ",L");
                        break;
                    case 3:
                        fprintf(fp, ",B");
                        break;
                    case 4:
                        fprintf(fp, ",S");
                        break;
                    case 5:
                        fprintf(fp, ",T");
                        break;
                    case 6:
                        fprintf(fp, ",F");
                        break;
                    case 8:
                        fprintf(fp, ",PC");
                        break;
                    case 9:
                        fprintf(fp, ",SW");
                        break;
                }
            }
            fputc(10, fp);
        }
        else if(formatFinder(objVector[index].substr(0,2))==1){//Format 1
            fputc(10, fp);
        }

        //Check to see if next command is base relative or PC relative
        if(objVector[index+1] != "M" && formatFinder(objVector[index+1].substr(0,2))==3 && formatFinder(objVector[index].substr(0,2))==3){ // if not M and if present and next instruction are format 3 then:
            if(nixbpeFinder(objVector[index+1].substr(0,3)).substr(3,1)=="1" && nixbpeStr.substr(4,1) == "1"){ // check if something in symtab
                string name = findAddressInSymtab(symHead, decimalToHex(baseAddress))->getName();
                fprintf(fp, "         BASE    %s", name.c_str()); // prints BASE and then the thing in symtab
                fputc(10, fp);
            }
            else if(nixbpeFinder(objVector[index+1].substr(0,3)).substr(4,1)=="1" && nixbpeStr.substr(3,1) == "1"){ // if current is base relative and if next one is pc relative then:
                fprintf(fp, "         NOBASE"); // No base
                fputc(10, fp);
            }

        }
        address += objVector[index++].length()/2;
    }
    /*
    After running out of text records,(the end of the program is not reacherd(checked by the lenth)) in this case:
        1. Check the symtab and then the littab(not really needed) to see if there is anything remaining that is greater than or equla to current address. If so:
            a. Do reserve bytes
        2. If not, write the end record.
    */

    Symbol *tmpSym = symHead;

    while(tmpSym!=nullptr){

        if(tmpSym->getDecValue() == address){

            int RESBlength = 0;
            int RESWlength = 0;

            if(tmpSym->next != nullptr) RESBlength = tmpSym->next->getDecValue() - tmpSym->getDecValue();
            else RESBlength = hexToDecimal(objVector[1].substr(12,6)) - tmpSym->getDecValue();
            if(RESBlength % 3 == 0) RESWlength = RESBlength / 3;
            string RESBstring = toString(RESBlength);
            string RESWstring = toString(RESWlength);
            
            if((RESBlength) % 3 == 0) fprintf(fp, "%s   RESW    %s", tmpSym->getName().c_str() ,RESWstring.c_str());
            else fprintf(fp, "%s   RESB    %s", tmpSym->getName().c_str() ,RESBstring.c_str());
            
            fputc(10, fp);
            address += RESBlength;
        }
        
        tmpSym = tmpSym->next;
    }

    //End line
    tmpSym = findAddressInSymtab(symHead, objVector[objVector.size()-1]);

    if(tmpSym != nullptr) fprintf(fp, "         END     %s", tmpSym->getName().c_str());
    else fprintf(fp, "         END   %s", objVector[objVector.size()-1].c_str());
    fputc(10, fp);
    


    
    //1) 1st Line of SIC File
    //      a) Extract Title of program and immediatly put into file
    //      b) Take first 6 characters to put directly into the program
    //      c) Put in three spaces (32 dec) 
    //      d) Put in START (83 84 65 82 84 dec)
    //      e) Put in three spaces (32 dec) 
    //      e) Put in starting address found in next six characters and insert 0A
    //      f) Go next line in objvector
    // 2) While Loop for machine instructions:
    //      i)  save current address in temp var
    //      ii) Pass address into a check for symtab and lit-tab coorespondance
    //      iii)If there is in the symtab, then copy the name into the leftmost column (6 characters) and two space characters (32)
    //      iv) If there is no symtab cooresponadance, put 8 spaces (32)
    //      v)  Check to see if obj code is extended
    //          a) do by passing first 3 characters into nixbpeFinder() function if its format 3 or 
    //                 b) if it isn't format 3 or 4, put in a space
    //      vi) Write in corresponding opcode from obj vector (6 characters) and space character
    //      vii) Check to see if indirect or immediate
    //              a) if indirect: add @
    //              b) if immediate: add #
    //              c) if both ticked: add space
    //              d) If a literal address corresponds to current address, we copy the literal into this bit and we can skip step 8
    //      viii) Put in the operand
    //          a) Check to see if the operand cooresponds to anything in symtab or littab
    //          b) Check to see if indexed
    //              i) if it is, add ",X" to end of it
    //          c) Add 0A
    //3) While Loop for reservations:
    //      i) Iterate accross symtab
    //          a) Get difference between value stored and next value stored 
    //              i) Reserve that many Bytes
    //                  a) Place Symbol label in first 6 columns, followed by two spaces, and RESB and difference
    //          a) For last symtab label, find difference between program length and address
}       

/*************************************************************
 FUNCTION: writeAddress(FILE *fp, int address)
 DESCRIPTION: converts decimal address to hex address for the lis file
 I/O:
    input parameters: FILE pointer, int address
    output: n/a
 *************************************************************/
void writeAddress(FILE *fp, int address){

    string s = decimalToHex(address);
    
    fprintf(fp, "%s  ", s.substr(s.length()-4,4).c_str());
}

/*************************************************************
 FUNCTION: writeAddress(FILE *fp, int address)
 DESCRIPTION: converts decimal address to hex address for the lis file
 I/O:
    input parameters: FILE pointer, int address
    output: n/a
 *************************************************************/
void writeOpcode(FILE *fp, string opcode, int column){

    while(column < 26){
        fputc(32, fp);
        column++;
    }

    fprintf(fp, "%s", opcode.c_str());
}


/*************************************************************
 FUNCTION: writeLisFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead)
 DESCRIPTION: writes the lis file based on the info provided
 I/O:
    input parameters: FILE pointer, vector<string>, Symbol pointer, Literal pointer
    output: n/a
 *************************************************************/
void writeLisFile(FILE *fp, vector<string> objVector, Symbol *symHead, Literal *litHead){
    //Instets 1st line of SIC program
    fprintf(fp, "%s  ", objVector[1].substr(8,4).c_str());
    fprintf(fp, "%s", objVector[1].substr(0,6).c_str());
    fprintf(fp, "   START   ");
    fprintf(fp, "%s", objVector[1].substr(6,6).c_str());
    fputc(10, fp);
     
    //Text records
    int index = 3;
    int address = hexToDecimal(objVector[1].substr(6,6));;
    int baseAddress = 0;
    int column = 0;
    bool baseBool = false;
    Symbol *symPtr = symHead; // Potential optimization
    Literal *litPtr = litHead; // Potential optimization
    string nixbpeStr = "";
    while(objVector[index] != "M"){

        writeAddress(fp, address);

        //First 8 columns(1 based)
        if(symPtr != nullptr && symPtr->getDecValue() == address){
            fprintf(fp, "%s  ", symPtr->getName().c_str());
            symPtr = symPtr->next;
        }
        else fprintf(fp, "        ");
        column += 8;

        //Column 9(1 based)
        if(formatFinder(objVector[index].substr(0,2))==3 && opcodeValid(objVector[index].substr(0,3))){
            nixbpeStr = nixbpeFinder(objVector[index].substr(0,3));
            if(nixbpeStr.substr(5,1) == "1") fprintf(fp, "+");//extended
            else fprintf(fp," ");
        }else fprintf(fp, " ");
        column ++;

        //Columns 10-16(1 based)
        if(litPtr!=nullptr && litPtr->getDecAddress()==address){
            fprintf(fp, "LTORG  ");
            fputc(10, fp);
            column = 0;
            while(litPtr!=nullptr && litPtr->getDecAddress()==address){
                writeAddress(fp, address);

                fprintf(fp, "*       %s", litPtr->getName().c_str());
                column += 7 + litPtr->getName().length();

                writeOpcode(fp, objVector[index], column);

                fputc(10, fp);
                column = 0;

                address += objVector[index++].length()/2;
                litPtr = litPtr->next;
            }
            continue;
        }
        fprintf(fp, "%s ", hexToCommand(objVector[index].substr(0,2)).c_str()); // General case
        column += hexToCommand(objVector[index].substr(0,2)).length();

        //Column 17
        if(formatFinder(objVector[index].substr(0,2))==3){
            nixbpeStr = nixbpeFinder(objVector[index].substr(0,3));
            if(nixbpeStr.substr(0,1)=="0") fputc(35, fp);
            else if(nixbpeStr.substr(1,1)=="0") fputc(64, fp);
            else fputc(32, fp);
            column++;
        }

        //Base Register checker
        if(hexToCommand(objVector[index].substr(0,2))=="LDB   "){
            int checkNibbles = 3;
            int checkAddress = 0;
            if(nixbpeStr.substr(5,1)=="1") checkNibbles = 5;
            else if(nixbpeStr.substr(4,1)=="1") checkAddress = address;
            else if(nixbpeStr.substr(3,1)=="1") checkAddress = baseAddress;

            if(nixbpeStr.substr(1,1)=="1") baseAddress = checkAddress + hexToDecimal(objVector[index].substr(3,checkNibbles));
            
        }

        //Columns 18-35
        /*
            We have to determine the address from the symtable. We know if its a direct address, immediate address, base relative or pc relative etc.
            We dont know if exactly where the BASE assembler directive would be. If LDB is every given, we can very well assume that the very next instruction is going to be to base relative.
            Take opcode and check the format.
            Depending of the format, the structure will change:
                * If Format 3 and 4 a lot of bs
                * If format 2, interpret the next 2 nibbles  of the opcode and put the corresponding register to the correct spots and add in 0A.
                * Format 1, do nothing and add 0A.
        */
        if(objVector[index].substr(0,2)=="4F"){
            writeOpcode(fp, objVector[index], column);
            fputc(10, fp);
        }
        else if(formatFinder(objVector[index].substr(0,2))==3){ // Format 3/4
            if(nixbpeStr.substr(5,1)=="0"){ // if not extended
                int tmpAddress = -1; // placeholder
                
                if(nixbpeStr.substr(4,1)=="1") tmpAddress = address + 3 + signedHexToDecimal(objVector[index].substr(3,3)); // pc relative
                else if(nixbpeStr.substr(3,1)=="1") tmpAddress = baseAddress + hexToDecimal(objVector[index].substr(3,3)); // base relative
                else tmpAddress = hexToDecimal(objVector[index].substr(3,3)); // nether base nor pc realtive
                
                Symbol *tmpSymPtr = findAddressInSymtab(symHead, decimalToHex(tmpAddress)); // check if in symtab
                Literal *tmpLitPtr = findAddressInLittab(litHead, decimalToHex(tmpAddress-3)); // check if in littab
                
                if(tmpSymPtr != nullptr){
                    string s = concatTrailingSpaces(tmpSymPtr->getName());
                    column -= tmpSymPtr->getName().length() - s.length();
                    fprintf(fp, "%s", s.c_str()); // if in symtab print out symbol name
                    column += tmpSymPtr->getName().length();
                }
                else if(tmpLitPtr != nullptr){
                    string s = concatTrailingSpaces(tmpLitPtr->getName());
                    column -= tmpLitPtr->getName().length() - s.length();
                    fprintf(fp, "%s", s.c_str()); // else if in littab, print litname
                    column += tmpLitPtr->getName().length();
                }
                else{
                    fprintf(fp, "%s", objVector[index].substr(3,3).c_str()); // else print remaining info
                    column += objVector[index].substr(3,3).length();
                } 
     
            }
            else { // if extended i.e. not relative
                Symbol *tmpSymPtr = findAddressInSymtab(symHead, "0"+objVector[index].substr(3,5)); // check in symtab
                Literal *tmpLitPtr = findAddressInLittab(litHead, "0"+objVector[index].substr(3,5)); // check if in littab

                if(tmpSymPtr != nullptr){
                    string s = concatTrailingSpaces(tmpSymPtr->getName());
                    column -= tmpSymPtr->getName().length() - s.length();
                    fprintf(fp, "%s", s.c_str()); // if in symtab print out symbol name
                    column += tmpSymPtr->getName().length();
                }
                else if(tmpLitPtr != nullptr){
                    string s = concatTrailingSpaces(tmpLitPtr->getName());
                    column -= tmpLitPtr->getName().length() - s.length();
                    fprintf(fp, "%s", s.c_str()); // else if in littab, print litname
                    column += tmpLitPtr->getName().length();
                }
                else{
                    fprintf(fp, "%s", objVector[index].substr(3,5).c_str()); // else print remaining info
                    column += objVector[index].substr(3,5).length();
                } 
            }
            

            if(nixbpeStr.substr(2,1)=="1"){
                fprintf(fp, ",X"); // check if indexed
                column+=2;
            } 
            
            writeOpcode(fp, objVector[index], column);

            fputc(10, fp);
            column = 0;
        }
        else if(formatFinder(objVector[index].substr(0,2))==2){//Format 2
            column ++;
            int x = hexToDecimal(objVector[index].substr(2,1));
            switch(x){
                case 0:
                    fprintf(fp, "A");
                    break;
                case 1:
                    fprintf(fp, "X");
                    break;
                case 2:
                    fprintf(fp, "L");
                    break;
                case 3:
                    fprintf(fp, "B");
                    break;
                case 4:
                    fprintf(fp, "S");
                    break;
                case 5:
                    fprintf(fp, "T");
                    break;
                case 6:
                    fprintf(fp, "F");
                    break;
                case 8:
                    fprintf(fp, "PC");
                    break;
                case 9:
                    fprintf(fp, "SW");
                    break;
            }
            if(objVector[index].substr(0,2)!="B0"&&objVector[index].substr(0,2)!="B4"&&objVector[index].substr(0,2)!="B8"){ // check if second argument
                column += 2;
                x = hexToDecimal(objVector[index].substr(3,1));
                switch(x){
                    case 0:
                        fprintf(fp, ",A");
                        break;
                    case 1:
                        fprintf(fp, ",X");
                        break;
                    case 2:
                        fprintf(fp, ",L");
                        break;
                    case 3:
                        fprintf(fp, ",B");
                        break;
                    case 4:
                        fprintf(fp, ",S");
                        break;
                    case 5:
                        fprintf(fp, ",T");
                        break;
                    case 6:
                        fprintf(fp, ",F");
                        break;
                    case 8:
                        fprintf(fp, ",PC");
                        break;
                    case 9:
                        fprintf(fp, ",SW");
                        break;
                }
            }
            writeOpcode(fp, objVector[index], column);
            fputc(10, fp);
            column = 0;
        }
        else if(formatFinder(objVector[index].substr(0,2))==1){//Format 1
            writeOpcode(fp, objVector[index], column);
            fputc(10, fp);
            column = 0;
        }

        //Check to see if next command is base relative or PC relative
        if(objVector[index+1] != "M" && formatFinder(objVector[index+1].substr(0,2))==3 && formatFinder(objVector[index].substr(0,2))==3){ // if not M and if present and next instruction are format 3 then:
            if(nixbpeFinder(objVector[index+1].substr(0,3)).substr(3,1)=="1" && !baseBool){ // check if something in symtab
                writeAddress(fp, address);
                string name = findAddressInSymtab(symHead, decimalToHex(baseAddress))->getName();
                fprintf(fp, "         BASE    %s", name.c_str()); // prints BASE and then the thing in symtab
                fputc(10, fp);
                column = 0;
                baseBool = true;
            }
            else if(nixbpeFinder(objVector[index+1].substr(0,3)).substr(4,1)=="1" && baseBool){ // if current is base relative and if next one is pc relative then:
                writeAddress(fp, address);
                fprintf(fp, "         NOBASE"); // No base
                fputc(10, fp);
                column = 0;
                baseBool = false;
            }

        }
        address += objVector[index++].length()/2;
    }
    /*
    After running out of text records,(the end of the program is not reacherd(checked by the lenth)) in this case:
        1. Check the symtab and then the littab(not really needed) to see if there is anything remaining that is greater than or equla to current address. If so:
            a. Do reserve bytes
        2. If not, write the end record.
    */

    Symbol *tmpSym = symHead;

    while(tmpSym!=nullptr){

        if(tmpSym->getDecValue() == address){

            int RESBlength = 0;
            int RESWlength = 0;

            if(tmpSym->next != nullptr) RESBlength = tmpSym->next->getDecValue() - tmpSym->getDecValue();
            else RESBlength = hexToDecimal(objVector[1].substr(12,6)) - tmpSym->getDecValue();
            if(RESBlength % 3 == 0) RESWlength = RESBlength / 3;
            string RESBstring = toString(RESBlength);
            string RESWstring = toString(RESWlength);

            writeAddress(fp, address);
            if((RESBlength) % 3 == 0) fprintf(fp, "%s   RESW    %s", tmpSym->getName().c_str() ,RESWstring.c_str());
            else fprintf(fp, "%s   RESB    %s", tmpSym->getName().c_str() ,RESBstring.c_str());
            fputc(10, fp);
            address += RESBlength;
        }
        
        tmpSym = tmpSym->next;
    }

    //End line
    tmpSym = findAddressInSymtab(symHead, objVector[objVector.size()-1]);

    fprintf(fp, "      ");

    if(tmpSym != nullptr) fprintf(fp, "         END     %s", tmpSym->getName().c_str());
    else fprintf(fp, "         END   %s", objVector[objVector.size()-1].c_str());
    fputc(10, fp);

}

int main(int argc, char* argv[]){
    //Case 1: there are no arguments passed
    if(argc != 2)gracefulExit("Fatal Error: no filename given.");//exit()

    //Case 2: obj or sym file dne
    string objFile (argv[1]);
    string symFile = objFile + ".sym";
    string sicFile = objFile + ".sic";
    string lisFile = objFile + ".lis";
    objFile = objFile + ".obj";
    if(!fileExists(objFile))gracefulExit("Fatal Error: object file not found.");//exit()
    if(!fileExists(symFile))gracefulExit("Fatal Error: symbol file not found.");//exit()

    Symbol *symHead = nullptr;//Create sym head and fp
    Literal *litHead = nullptr;
    FILE *fp = fopen(symFile.c_str(), "r");
    symHead = toSymbol(symHead, fp);//pass to toSymbol
                            //check if next byte exists w/ while(!feof(fp)){}
    litHead = toLiteral(litHead, fp);

    closeFile(fp);
    /*
    cout << symHead->getValue() << endl;
    cout << symHead->next->getValue() << endl;
    cout << litHead->getAddress() << endl;
    */
    // READING OBJ FILE BELOW
    FILE *fpObj = fopen(objFile.c_str(), "r");
    vector<string> objectVector = readObj(fpObj, symHead, litHead);
    closeFile(fpObj);

    /* READING FROM OBJ FILE

        Datastructure to store the info below:
            1. Vector for each line.

        1. Check if 1st character is H. Else GTFO.
            a. Take in next 6 characters as name of the program. In the first line.
            b. Take in next 6 characters. That is the first address for .lis instruction.
            c. Take in the next 6 characters. That is the lenght of the program.
            d. The next character is 0A(hex), i.e. move to next line.
        2. Check if the 1st character is T. Else go to 3.
            a. 1st 6 characters are the starting address in that record.
            b. Next 2 characters are the lenght of the text record
            c. From the next record to 0A, thats all opcode.(Look in modification record to spot Extended bits)
            d. Go to 2.
        3. Check if the 1st character is M. Else go to 4.
            a. First 6 characters are address of instruction to be modified.
            b. Next 2 characters are the numbers of characters in the machine instruction's opcode that must be modified.
            c. Store next characters till 0A. These characters are the (+-label) whose address we have to add(or sub) to the address of the instruction
            d. Go to 3.
        4. Check if the 1st character is E. Else ERROR, NO END RECORD.
            a. Do nothing
            
    */

    FILE* sicfp = createFile(sicFile);

    writeSicFile(sicfp, objectVector, symHead, litHead);

    closeFile(sicfp);


    FILE* lisfp = createFile(lisFile);

    writeLisFile(lisfp, objectVector, symHead, litHead);

    closeFile(lisfp);

    return 0;


}
