/*
    Pseduo code as of now:
        1. We check for correct input:
            a. Check for actual argument
            b. check for existence for .obj and .sym file
        1.b If check fail, do graceful exit

        2. Initalize .sic and .lis files. Maybe make one more intermediatory file for debugging purposes.
            a. Create file via C++.

        3. Algorithm for disassembling
            a. Read the .obj and .sym file.
            b. Interpret the contents via code.
            c. Write line by line? or store in intermediatory file and then write to .lis and .sic.

        3.5  Writing it to files
            d. Write the .lis file
            e. Write the .sic file
*/


// Make Graceful exit function which includes tailored exit messages

/*
    Input                   Output
    <filename>.obj           <filename>.sic
    <filename>.sym           <filename>.lis

    % xed <filename>
    
*/

/*
    Tips from prof:

    Graceful Exit:
        exit() -> Test return value on every system call. Int error handling routine
    
    Don't use templates in C++.
    Use C versions of I/O commands
*/