int main() {
  int h = 0;
  bigint x, q;
  char *buf1, *buf2;
  while (scanf(" %as %as", &buf1, &buf2) != EOF) {
    x = bigint(buf1);
    q = x.div(bigint(buf2));
    printf("#%d\n", ++h);
    printf("%s / %s = %s\n", buf1, buf2, string(q).c_str());
    printf("%s %% %s = %s\n", buf1, buf2, string(x).c_str());
    printf("\n");
    free(buf1);
    free(buf2);
  }
}
