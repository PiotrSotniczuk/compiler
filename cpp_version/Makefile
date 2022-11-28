CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o bnfc/Skeleton.o
LATCO=latc.o
FRONTO=Front.o
FINDDEF=Find_def.o
SHARED=Shared.o
MY_FILES=${FRONTO} ${FINDDEF} ${SHARED} ${LATCO}

all: latc

latc: ${BNFC_FILES} ${MY_FILES}
	@echo "Compiling Latte..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${MY_FILES} -o latc

${LATCO} : src/Latte.cpp bnfc/Parser.H bnfc/Absyn.H bnfc/Skeleton.H src/Front.cpp src/Find_def.cpp src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Latte.cpp -o ${LATCO}

${FRONTO}: bnfc/Skeleton.H src/Front.cpp src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Front.cpp -o ${FRONTO}

${FINDDEF}: bnfc/Skeleton.H src/Find_def.cpp src/Shared.cpp src/Find_def.h
	${CC} ${CCFLAGS} -c src/Find_def.cpp -o ${FINDDEF}

${SHARED}: src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Shared.cpp -o ${SHARED}

clean : 
	rm -f *.o latc && cd bnfc/ && make clean && cd ..