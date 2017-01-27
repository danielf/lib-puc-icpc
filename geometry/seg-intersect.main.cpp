int main() {
  while (1) {
    char *buf;
    scanf(" %a[^\n]", &buf);
    if (buf[0] == '#') {
      free(buf);
      break;
    }
    point p[4];
    sscanf(buf, " %lf %lf %lf %lf %lf %lf %lf %lf", \
      &p[0].x, &p[0].y, &p[1].x, &p[1].y, \
      &p[2].x, &p[2].y, &p[3].x, &p[3].y);
    bool r = seg_intersect(p[0], p[1], p[2], p[3]);
    printf("%s\n", r ? "true" : "false");
    free(buf);
  }
  return 0;
}
