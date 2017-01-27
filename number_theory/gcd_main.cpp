int main() {
  int x, y;
  while (scanf(" %d %d", &x, &y) != EOF)
    printf("%d %d: %d\n", x, y, gcd(x, y));
  return 0;
}
