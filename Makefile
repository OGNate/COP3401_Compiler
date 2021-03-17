COMP = gcc

Compiler: CompilerMain.o LexAnalyzer.o ParserCodegen.o VM.o
	$(COMP) CompilerMain.o LexAnalyzer.o ParserCodegen.o VM.o -o Compiler

CompilerMain.o: CompilerMain.c LexAnalyzer.h ParserCodegen.h VM.h
	$(COMP) -c CompilerMain.c

LexAnalyzer.o: LexAnalyzer.c LexAnalyzer.h
	$(COMP) -c LexAnalyzer.c

ParserCodegen.o: ParserCodegen.c ParserCodegen.h CompilerMain.h LexAnalyzer.h VM.h
	$(COMP) -c ParserCodegen.c

VM.o: VM.c VM.h 
	$(COMP) -c VM.c

clean:
	@echo "clean project"
	-del -fR Compiler.exe *.o
	@echo "clean complete"
.PHONY: clean