#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printString(char *s) {
    printf("%s\n", s);
}

void printInt(int a) {
    printf("%d\n", a);
}

int readInt() {
    int x;
    scanf("%d", &x);
    return x;
}

char *readString(void) {
    char *line = NULL;
    size_t size = 0;
    scanf(" ");
    int len = getline(&line, &size, stdin);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';
    }
    return line;
}

void error() {
    puts("runtime error");
    exit(1);
}

// returns like boolean 1 if eqal 0 if different
int __compare_str(char *str1, char *str2){
    if(strcmp(str1, str2) == 0){
        return 1;
    }else{
        return 0;
    }     
}

//https://www.codeproject.com/Questions/5314795/How-do-I-concatenate-2-pointer-strings-correctly
char *__concat(char *A, char *B) {
    int lenA = strlen(A);
    int lenB = strlen(B);
    char *A3 = (char*) malloc(lenA + lenB + 1);
    memcpy(A3, A, lenA);
    memcpy(A3 + lenA, B, lenB);
    A3[lenA + lenB] = '\0';
    return A3;
}