CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o bnfc/Skeleton.o
LATC_O=latc.o
TYPE_CHECKER=Type_checker.o
FIND_DEF=Find_def.o
SHARED=Shared.o
FRONT_LIBS=${TYPE_CHECKER} ${FIND_DEF} ${SHARED} ${LATC_O}
BACK_LIBS=${BNFC_FILES}

all: latc latc_x86_64

latc: ${BNFC_FILES} ${FRONT_LIBS}
	@echo "Compiling LatteFront..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${FRONT_LIBS} -o latc

latc_x86_64: lib/runtime.o src/back/Compiler.cpp
	@echo "Compiling LatteBack..."
	${CC} ${CCFLAGS} ${BACK_LIBS} src/back/Compiler.cpp -o latc_x86_64

${LATC_O} : src/front/Latte.cpp
	${CC} ${CCFLAGS} -c src/front/Latte.cpp -o ${LATC_O}

${TYPE_CHECKER}: src/front/Type_checker.cpp
	${CC} ${CCFLAGS} -c src/front/Type_checker.cpp -o ${TYPE_CHECKER}

${FIND_DEF}: src/front/Find_def.cpp
	${CC} ${CCFLAGS} -c src/front/Find_def.cpp -o ${FIND_DEF}

${SHARED}: src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Shared.cpp -o ${SHARED}

lib/runtime.o: lib/runtime.c
	gcc -c lib/runtime.c -o lib/runtime.o

clean : 
	rm -f *.o latc latc_x86_64 lib/runtime.o && cd bnfc/ && make clean && cd ..