const int HPREC = 110; // folga
const int PREC = 2 * HPREC;

int main() {
  bigint pw = 1, hpw = 1;
  for (int i = 0; i < HPREC; i++) {
    hpw *= 10;
  }
  for (int i = 0; i < PREC; i++) {
    pw *= 10;
  }
  bigint a, pi = pw;
  a = 2;
  a *= pw;
  a = a.root(2);
  a *= hpw;
  for (int k = 0; k < PREC; k++) {
    pi *= a;
    pi /= 2;
    pi /= pw;
    a += pw;
    a += pw;
    a = a.root(2);
    a *= hpw;
  }
  bigint rpi = pw;
  rpi *= 2;
  rpi *= pw;
  rpi /= pi;
  char buf[101];
  strncpy(buf, string(rpi).c_str(), 100);
  printf("%c.%s\n", buf[0], buf + 1);
  return 0;
}
