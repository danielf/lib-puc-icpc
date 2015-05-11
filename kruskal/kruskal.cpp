#include "../template.cpp"
#include "unionfind.cpp"
#include <tuple>
using namespace std;

int main() {
	int n, m;
	scanf("%d %d", &n, &m);
	typedef tuple<int, int, int> edge;
	vector<edge> V(m);
	for (auto &x : V)
		scanf("%d %d %d", &get<1>(x), &get<2>(x), &get<0>(x));
	sort(all(V));
	int ans = 0;
	UF U(n);
	for (auto &x : V)
		if (U.unio(get<1>(x), get<2>(x)))
			ans += get<0>(x);
	printf("%d\n", ans);
	return 0;
}
