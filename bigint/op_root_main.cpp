int main() {
	int h = 0, n;
	bigint x;
	char *buf1;
	while (scanf(" %as %d", &buf1, &n) != EOF) {
		x = bigint(buf1);
		printf("#%d\n%s\n\n", ++h, string(x.root(n)).c_str());
		free(buf1);
	}
}
