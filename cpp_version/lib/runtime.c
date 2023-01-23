#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void _gcIncr(void* ptr);

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
    _gcIncr(line);
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
    _gcIncr(A3);
    return A3;
}

struct gcCounter {
  void* ptr;
  int count;
  struct gcCounter *next;
};

typedef struct gcCounter gcCounter;

static gcCounter *gcFirst = NULL;
static gcCounter *gcLast = NULL;

static gcCounter *_gcInit(void* ptr) {
  gcCounter *res = (gcCounter *)malloc(sizeof(gcCounter));
  res->ptr = ptr;
  res->count = 1;
  res->next = NULL;
  return res;
}

static gcCounter *_gcFind(void* ptr) {
  gcCounter *g = gcFirst;
  while (g != NULL) {
    if (g->ptr == ptr) {
      return g;
    }
    g = g->next;
  }
  return NULL;
}

void _gcIncr(void* ptr) {
  fprintf(stderr, "GC INCR: %d\n", (int)ptr);
  if (gcFirst == NULL) {
    gcFirst = gcLast = _gcInit(ptr);
    return;
  }
  gcCounter *g = _gcFind(ptr);
  if (g == NULL) {
    gcLast->next = _gcInit(ptr);
    gcLast = gcLast->next;
    return;
  }
  g->count++;
}

void _gcClean() {
  gcCounter *g = gcFirst;
  while (g != NULL) {
    gcCounter *next = g->next;
    free(g->ptr);
    free(g);
    g = next;
  }
}
