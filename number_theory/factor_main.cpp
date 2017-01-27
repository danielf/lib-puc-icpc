int main() {
  int n;
  while (scanf(" %d", &n) != EOF) {
    prime_map M = factor(n);
    printf("%d: ", n);
    foreach (it, M) {
      if (it != M.begin()) printf(" * ");
      printf("%d", it->first);
      if (it->second > 1) printf("**%d", it->second);
    }
    printf("\n");
  }
  return 0;
}
