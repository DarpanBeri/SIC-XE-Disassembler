<!-----------------------------------------------------------------
 Name: Blake Meyers, Darpan Beri, Zach Selchau
 Project: SIC-XE-Disassembler
 File: README.md
 Notes: A README file describing the program and detailing its files.
--------------------------------------------------------------------->

## SIC-XE-Disassembler Sofware Design Document
## Blake Meyers, Darpan Beri, Zach Selchau

### Software Design

1. Object Oriented Design
    * **Note**: We only employed OOD in internal datastructures.
    * OOD allowed easy storage of Symbols and Literals along with their respective properties.
2. Functional programing and practices
    * The program could be easily separated into chunks which worked best with functional programming. It allowed us to decrease the size of the main substantially.
3. Waterfall methodology
    * Because of the program's scope and time commitment required for a prototype, we decided to use the waterfall methodology.
4. Pair programming
    * To ensure Quality over quantity and to keep everyone updated we decided to use pair programming approach over diving the work between us.

### Comments about working as a team

* **Pros:**
    - Easier to catch and correct errors.
    - Allowed us to bounce ideas off each other.
    - Discussing implementations.
    - Kept each other updated.
    - Held each other accountable.
    - Teaching each other.
    - Every member has complete understanding of the project.
    - The project felt easier with friends.
    - Power of friendship.
* **Cons:**
    - Scheduling issues.
    - Difficult for multiple to update the same file at the same time.
    - Had to keep everyone up-to-date.

### Areas of improvement:

* Could have met more regularly.
* Could have identified the requirements of the project more clearly at the beginning.
* Could have asked Prof. Leonard for more clarification early on.
* Could have read the textbook.

### Meeting Days

    Thursdays - 4:00pm.
    Fridays - after AI Club (1:30pm)
    Saturdays - 1:00pm
    Sundays - 1:00pm

### Work Done Each Day

    March 3 - Initialized Git repository, added psuedo code, and created a schedule.
    March 8 - First line of code, added a file check and intialized graceful exit.
    March 17 - Updated README.md
    March 19 - Added HextoCommand and HextoDecimal Functions.
    March 20 - Updated HextoCommand.
    March 23 - Implemented exit failure, implemented graceful exits,created internal Littab and Symtab, and added more psuedo code.
    April 3 - Created algorithm for reading symtab and storing data. Added Linked List capabilities to internal Symbol and Literal datastructures.
    April 4 - Added algorithm to read from the Littab and store in internal datastructure.
    April 12 - Implemented algorithm to read object file.
    April 13 - Worked on reading the object file. Added function formatFinder().
    April 14 - Created outline for writing the .sic and .lis file.
    April 15 - Fixed the order of symbol linked list and literal linked list. Began implementation for writing to .sic file.
    April 16 - Worked on the algorithm to write to the .sic file and formatting. Modified the implementation to read the .obj file.
    April 17 - Worked on writing to .sic file. Added address to symbol representation.
    April 18 - Completed writing to .sic, began writing to .lis. Completed formatting for .sic.
    April 19 - Completed the writing to .lis, added Makefile, updated README.md, code optimization, and refined Software Design Document.

<!-----------------------------------------[ EOF: SoftwareDesignDocument.md ]--------------------------------->
