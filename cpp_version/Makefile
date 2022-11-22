CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o
LATCO=latc_x86.o

all: latc_x86

insc_jvm: ${BNFC_FILES} ${LATCO}
	@echo "Compiling Latte..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${LATCO} -o latc_x86

${LATCO} : src/Latte.cpp bnfc/Parser.H bnfc/Absyn.H
	${CC} ${CCFLAGS} -c src/Latte.cpp

clean : 
	rm -f *.o latc_x86 && cd bnfc/ && make clean && cd ..