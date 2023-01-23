#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void __garbUp(void* ptr);

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
    __garbUp(line);
    
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
    __garbUp(A3);
   
    return A3;
}

// all string s should be freeable now
char* __allocString(char* str) {
  int size = strlen(str) + 1;
  char* new_str = (char*)malloc(size);
  __garbUp(new_str);
  
  strcpy(new_str, str);
  return new_str;
}

// I will keep reference counters on List
struct garbList {
  int refs;
  void* str_ptr;
  struct garbList *next;
};

typedef struct garbList garbList;

static garbList *garbStart = NULL;
static garbList *garbEnd = NULL;

static garbList *__garbInit(void* ptr) {
  garbList *res = (garbList *)malloc(sizeof(garbList));
  res->next = NULL;
  res->str_ptr = ptr;
  res->refs = 1;
  return res;
}

static garbList *__garbFind(void* ptr) {
  garbList *g = garbStart;
  while (g != NULL) {
    if (g->str_ptr == ptr) {
      return g;
    }
    g = g->next;
  }
  return NULL;
}

void __garbUp(void* ptr) {
  if (garbStart == NULL) {
    garbStart = __garbInit(ptr);
    garbEnd = garbStart;
    return;
  }
  garbList *g = __garbFind(ptr);
  if (g == NULL) {
    garbEnd->next = __garbInit(ptr);
    garbEnd = garbEnd->next;
    return;
  }
  g->refs++;
}

void __garbDown(void* ptr) {
  if (ptr == NULL) return;
  garbList *g = __garbFind(ptr);
  if (g == NULL) {
    fprintf(stderr, "Trying to decr not found ptr = %d\n", (int)ptr);
    return;
  }
  g->refs--;
  fprintf(stderr, "COUNT: %d\n", g->refs);
  // free 
  if (g->refs == 0) {
    free(g->str_ptr);
    fprintf(stderr, "Freeing ptr = %d\n", (int)g->str_ptr);
    garbList *next = g->next;
    free(g);
    // update list 
    if (g == garbStart) {
      garbStart = next;
      if (next == NULL) {
        garbEnd = NULL;
      }
      return;
    }
    garbList *p = garbStart;
    while (p->next != g) {
      p = p->next;
    }
    p->next = next;
    if (garbEnd == g) {
      garbEnd = p;
    }
  }
}

// this will be run at the end of main 
// this will destroy and free whole structure
// if there are no mistakes in code than this should do anything
void __garbPurge() {
  garbList *g = garbStart;
  while (g != NULL) {
    garbList *next = g->next;
    free(g->str_ptr);
    free(g);
    g = next;
  }
}

void __printLen(){
  int i = 0;
  garbList *g = garbStart;
  while(g != NULL){
    i++;
    g = g->next;
  }
  fprintf(stderr, "List is size = %d\n", i);
}