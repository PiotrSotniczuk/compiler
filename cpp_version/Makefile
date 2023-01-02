CC=g++
CCFLAGS=-g -W -Wall -std=c++17

BNFC_FILES=bnfc/Absyn.o bnfc/Lexer.o bnfc/Parser.o bnfc/Skeleton.o
TYPE_CHECKER=Type_checker.o
FIND_DEF=Find_def.o
SHARED=Shared.o
FRONT_LIBS=${TYPE_CHECKER} ${FIND_DEF} ${SHARED}
COMPILER=Compiler.o
BACK_LIBS=${BNFC_FILES} ${COMPILER} ${FIND_DEF} ${SHARED}

all: latc latc_x86_32

latc: ${BNFC_FILES} ${FRONT_LIBS} src/front/Latte.cpp
	@echo "Compiling LatteFront..."
	${CC} ${CCFLAGS} ${BNFC_FILES} ${FRONT_LIBS} src/front/Latte.cpp -o latc

latc_x86_32: lib/runtime.o src/back/Back.cpp ${BACK_LIBS}
	@echo "Compiling LatteBack..."
	${CC} ${CCFLAGS} ${BACK_LIBS} src/back/Back.cpp -o latc_x86_32

${TYPE_CHECKER}: src/front/Type_checker.cpp
	${CC} ${CCFLAGS} -c src/front/Type_checker.cpp -o ${TYPE_CHECKER}

${FIND_DEF}: src/front/Find_def.cpp ${SHARED}
	${CC} ${CCFLAGS} -c ${SHARED} src/front/Find_def.cpp -o ${FIND_DEF}

${SHARED}: src/Shared.cpp
	${CC} ${CCFLAGS} -c src/Shared.cpp -o ${SHARED}

${COMPILER}: src/back/Compiler.cpp
	${CC} ${CCFLAGS} -c src/back/Compiler.cpp -o ${COMPILER}

lib/runtime.o: lib/runtime.c
	gcc -c lib/runtime.c -o lib/runtime.o -m32 -no-pie

clean : 
	rm -f *.o latc latc_x86_32 lib/runtime.o && cd bnfc/ && make clean && cd ..