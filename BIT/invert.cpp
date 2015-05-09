#include "../template.cpp"
#include "bit.cpp"

int main() {
	int N;
	scanf("%d", &N);
	vi V(N);
	for (int &x : V) scanf("%d", &x);
	BIT b(N);
	int ans = 0;
	for (int x : V) {
		ans += b.get(x, N-1);
		b.update(x, 1);
	}
	printf("%d\n", ans);
	return 0;
}
