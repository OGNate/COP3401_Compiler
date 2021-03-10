COMP = gcc

Compiler: CompilerMain.o LexAnalyzer.o
	$(COMP) CompilerMain.o LexAnalyzer.o -o Compiler

CompilerMain.o: CompilerMain.c LexAnalyzer.h
	$(COMP) -c CompilerMain.c

LexAnalyzer.o: LexAnalyzer.c LexAnalyzer.h
	$(COMP) -c LexAnalyzer.c

clean:
	@echo "clean project"
	-del -fR Compiler.exe *.o
	@echo "clean complete"
.PHONY: clean