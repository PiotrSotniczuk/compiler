void error() {
  puts("runtime error");
  exit(1);
}

void printInt(Int64 x) { printf("%lld\n", x); }

void printString(String s) { puts(s); }

void _free(Ptr ptr) {
  fprintf(stderr, "FREEING %lld\n", (Int64)ptr);
  free(ptr);
}

Int64 readInt() {
  Int64 result;
  scanf("%lld  ", &result);
  return result;
}

String readString() {
  String res = NULL;
  size_t n = 0;
  getline(&res, &n, stdin);
  size_t len = strlen(res);
  if (len > 0 && res[len - 1] == '\n') {
    res[len - 1] = '\0';
  }
  _gcIncr(res);
  return res;
}