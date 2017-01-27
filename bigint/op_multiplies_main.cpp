int main() {
  int h = 0;
  bigint x;
  char *buf1, *buf2;
  while (scanf(" %as %as", &buf1, &buf2) != EOF) {
    x = bigint(buf1);
    x *= bigint(buf2);
    printf("#%d\n%s\n\n", ++h, string(x).c_str());
    free(buf1);
    free(buf2);
  }
}
