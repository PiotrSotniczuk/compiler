CC=g++ -g
CCFLAGS=--ansi -W -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unneeded-internal-declaration

FLEX=flex
FLEX_OPTS=-PLatteCPP

BISON=bison
BISON_OPTS=-t -pLatteCPP

OBJS=Absyn.o Buffer.o Lexer.o Parser.o Printer.o

.PHONY : clean distclean

all : TestLatteCPP

clean :
	rm -f *.o TestLatteCPP LatteCPP.aux LatteCPP.log LatteCPP.pdf LatteCPP.dvi LatteCPP.ps LatteCPP

distclean : clean
	rm -f Absyn.C Absyn.H Buffer.C Buffer.H Test.C Parser.C Parser.H ParserError.H LatteCPP.y Lexer.C LatteCPP.l Skeleton.C Skeleton.H Printer.C Printer.H Makefile LatteCPP.tex

TestLatteCPP : ${OBJS} Test.o
	@echo "Linking TestLatteCPP..."
	${CC} ${OBJS} Test.o -o TestLatteCPP

Absyn.o : Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Buffer.o : Buffer.C Buffer.H
	${CC} ${CCFLAGS} -c Buffer.C

Lexer.C : LatteCPP.l
	${FLEX} -oLexer.C LatteCPP.l

Parser.C : LatteCPP.y
	${BISON} LatteCPP.y -o Parser.C

Lexer.o : Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C

Parser.o : Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o : Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o : Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c Skeleton.C

Test.o : Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C
