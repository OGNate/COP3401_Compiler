COMP = gcc

Compiler: CompilerMain.o LexAnalyzer.o ParserCodegen.o
	$(COMP) CompilerMain.o LexAnalyzer.o ParserCodegen.o -o Compiler

CompilerMain.o: CompilerMain.c LexAnalyzer.h ParserCodegen.h
	$(COMP) -c CompilerMain.c

LexAnalyzer.o: LexAnalyzer.c LexAnalyzer.h
	$(COMP) -c LexAnalyzer.c

ParserCodegen.o: ParserCodegen.c ParserCodegen.h CompilerMain.h LexAnalyzer.h
	$(COMP) -c ParserCodegen.c

clean:
	@echo "clean project"
	-del -fR Compiler.exe *.o
	@echo "clean complete"
.PHONY: clean