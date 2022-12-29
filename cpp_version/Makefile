CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o bnfc/Skeleton.o
LATC_O=latc.o
TYPE_CHECKER=Type_checker.o
FIND_DEF=Find_def.o
SHARED=Shared.o
FRONT_LIBS=${TYPE_CHECKER} ${FIND_DEF} ${SHARED} ${LATC_O}

all: latc

latc: ${BNFC_FILES} ${FRONT_LIBS}
	@echo "Compiling LatteFront..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${FRONT_LIBS} -o latc

latc_x86: lib/runtime.o 
	@echo "Compiling LatteBack..."
	${cc} ${CCFLAGS} lib/runtime.o  -o latc_x86

${LATC_O} : src/front/Latte.cpp bnfc/Parser.H bnfc/Absyn.H bnfc/Skeleton.H src/front/Type_checker.cpp src/front/Find_def.cpp src/Shared.cpp
	${CC} ${CCFLAGS} -c src/front/Latte.cpp -o ${LATC_O}

${TYPE_CHECKER}: bnfc/Skeleton.H src/front/Type_checker.cpp src/Shared.cpp
	${CC} ${CCFLAGS} -c src/front/Type_checker.cpp -o ${TYPE_CHECKER}

${FIND_DEF}: bnfc/Skeleton.H src/front/Find_def.cpp src/Shared.cpp src/front/Find_def.h
	${CC} ${CCFLAGS} -c src/front/Find_def.cpp -o ${FIND_DEF}

${SHARED}: src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Shared.cpp -o ${SHARED}

lib/runtime.o: lib/runtime.cpp
	${CC} ${CCFLAGS} -c lib/runtime.cpp -o lib/runtime.o

clean : 
	rm -f *.o latc latc_x86 && cd bnfc/ && make clean && cd ..