#include <stdio.h>
// #include <string.h>
// #include <limits.h>
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

// char *__concat(char *str1, char *str2) {
//     char *res;
//     size_t l1 = strlen(str1);
//     size_t l2 = strlen(str2);
//     int len = l1 + l2 + 1;
//     res = malloc(len);
//     memcpy(res, str1, l1);
//     memcpy(res + l1, str2, l2);
//     res[l1 + l2] = '\0';
//     return res;
// }