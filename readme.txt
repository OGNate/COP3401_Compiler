Contributor:
    - Dillon Garcia
    - Nathan Wilson

How to run our code:

    1) Make sure that all cooresponding source code files (.c and .h), test case files (.txt), and the Makefile are in the some folder/directory.
        -> Source Code files
            - CompilerMain.c and CompilerMain.h
            - LexAnalyzer.c and LexAnalyzer.h
            - ParserCodegen.c and ParserCodegen.h
            - VM.c and VM.h

        -> Test Case files
            - test1.txt and test2.txt
            - ANY OTHER TEST FILES 

        -> Make file
            - Makefile

    2) Make sure that you are in the directory with all the files mentioned above.

    3) Run the "make" command in the Eustis command prompt

    4) Information on running the executable

        The command line takes a couple of arguments after ./Compiler

        a) The second command line argument must be the name of the input file.
            ex:
                -> test1.txt

        b) The third, fourth, and fifth command line arguments are for directives
            -> "-l" directive:
                - Prints the output of the Lexical Analyzer

            -> "-a" directive:
                - Prints the output of the Parser and Code generator

            -> "-v" directive:
                - Prints the output of the Virtual Machine

        ** The command line can take none of the directives, some of the directives, or all of the directives **
            -> Ex:
                - " "
                - "-l -v"
                - "-l -a -v"
    
    5) Running the executable

        - The format of the execution should be:
            "<Executable Name> <input text file> <Any directives>"

        - Running test1.txt with all the directives:
            "./Compiler test1.txt -l -a -v"

        - Running test1.txt with some of the directives:
            "./Compiler test1.txt -l -v"

        - Running test1.txt with no directives:
            "./Compiler test1.txt"

        

        

            
        
        

