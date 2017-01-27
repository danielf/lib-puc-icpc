int main() {
  int p;
  while (scanf(" %d", &p) != EOF) {
    bigint x = 4, m = 1;
    for (int i = 0; i < p; i++) m *= 2;
    m -= 1;
    for (int i = 0; i < p - 2; i++) {
      x *= x;
      x -= 2;
      x %= m;
    }
    printf("%d: ", p);
    if (x.cmp()) printf("no\n");
    else printf("yes\n");
  }
  return 0;
}
