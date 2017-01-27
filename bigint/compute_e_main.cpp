const int PREC = 110;

int main() {
  bigint pw = 1;
  for (int i = 0; i < PREC; i++) {
    pw *= 10;
  }
  bigint e = 0, t = 1;
  e *= pw;
  t *= pw;
  for (int i = 0; t.cmp(); i++) {
    e += t;
    t /= i + 1;
  }

  char buf[101];
  strncpy(buf, string(e).c_str(), 100);
  printf("%c.%s\n", buf[0], buf + 1);
  return 0;
}
