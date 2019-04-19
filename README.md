<!-----------------------------------------------------------------
 Name: Blake Meyers(cssc0430), Darpan Beri(cssc0429), Zach Selchau(cssc0418)
 Project: CS530 Assignment 2
 File: README.md
 Notes: A README file describing the program and detailing its files.
--------------------------------------------------------------------->

## CS-530, Spring 2019 Assignment 2
### Developed by Blake Meyers(cssc0430), Darpan Beri(cssc0429), Zach Selchau(cssc0418)

#### The Program:
    Opens an XE object file, and its accompanying symbol file.
    Then it will disassemble the object code, and generate an XE source file, and XE listing file using the disassembled code.

#### Files Included:
**README.md**:

    A README file describing the program and detailing its files.

**xed.cpp**:

    The file cointains an algorithm that opens an XE object file and its accompanying symbol file, which then disassembles the object code, and generate an XE source file, and XE listing file using the disassembled code.

**xed.hpp**:

    Declares and briefly explains functions implemented in xed.cpp.

**Makefile**:

    Creates a compiled version of xed.cpp.

#### Compilation Instructions:
**Make Instructions:**

    make all:
    	compiles the xbd file

    make clear:
    	deletes the xbd file

**Running the Program:**

    xed <filename>

**NOTE:** <> are not part of the command.

#### Operating Instructions:
**Required Input**:

    The input filename needs to be the filename of a .obj and a .sym file.
        The .obj file needs to be an object file of SIC/XE machine.
        The .sym file needs to be a symboltable and littable file for the SIC/XE machine,

#### Significant Design Decisions:
    add stuff here

#### Extra features:
**add stuff here:**

    add stuff here

#### Known Deficiencies and Bugs:
**Known Bugs and Deficiencies**
    add stuff here

#### Lessons Learned:
* An algorithm and design takes 1 month to figure out. The actual implementation requires 1 week.
* Quality of work matters over the quantity of work done.

<!-----------------------------------------[ EOF: README.md ]--------------------------------->