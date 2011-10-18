#include <stdio.h>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Calcula o maior divisor comum dos números x e y.
//

int gcd(int x, int y) { return y ? gcd(y, x % y) : abs(x); }

////////////////////////////////////////////////////////////////////////////////
// Calcula o mínimo múltiplo comum de a e b.
//

unsigned long long lcm(int x, int y) {
	if (x && y) return abs(x) / gcd(x, y) * (unsigned long long)(abs(y));
	else return (unsigned long long)(abs(x | y));
}

#include "../trc.cpp"

int main() {
	vector< pair<int,int> > v;

	int n, expected;
	while (scanf(" %d %d", &n, &expected) == 2 && n) {
		v.clear();
		int N = 1;
		for (int i=0; i < n; i++) {
			int ai, mi;
			scanf(" %d %d", &ai, &mi);
			printf("x = %d (mod %d)\n", ai, mi);
			v.push_back( make_pair(ai,mi) );
			N *= mi;
			//N = lcm(N, mi);
		}
		printf("----------\n");
		int x = TRC(v);
		printf("x = %d (mod %d)\n", x, N);
		if (x == expected) printf("OK!\n");
		else printf("FAIL! (expected %d)\n", expected);
		printf("\n");
	}

	return 0;
}
