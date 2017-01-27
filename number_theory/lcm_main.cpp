int main() {
  int x, y;
  while (scanf(" %d %d", &x, &y) != EOF)
    printf("%d %d: %lld\n", x, y, lcm(x, y));
  return 0;
}
