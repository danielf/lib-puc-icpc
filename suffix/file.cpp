#include "../template.cpp"
#include "suffix.cpp"

int main() {
	while (true) {
		string s;
		cin >> s;
		if (s[0] == '*') break;
		suffix S(s);
		vi order = S.order();
		int ans = 0;
		int cur = 0;
		fu(i, sz(s) - 1) {
			int tmp = S.lcp(order[i], order[i+1]);
			if (tmp > cur) ans += tmp - cur;
			cur = tmp;
		}
		printf("%d\n", ans);
	}
	return 0;
}
