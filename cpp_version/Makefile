CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o bnfc/Skeleton.o
LATCO=latc_x86.o
FRONTO=Front.o

all: latc_x86

latc_x86: ${BNFC_FILES} ${LATCO} ${FRONTO}
	@echo "Compiling Latte..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${FRONTO} ${LATCO} -o latc_x86

${LATCO} : src/Latte.cpp bnfc/Parser.H bnfc/Absyn.H bnfc/Skeleton.H src/Front.h
	${CC} ${CCFLAGS} -c src/Latte.cpp -o ${LATCO}

${FRONTO}: bnfc/Skeleton.H
	${CC} ${CCFLAGS} -c src/Front.cpp -o ${FRONTO}

clean : 
	rm -f *.o latc_x86 && cd bnfc/ && make clean && cd ..