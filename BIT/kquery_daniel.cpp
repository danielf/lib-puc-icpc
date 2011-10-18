#include <stdio.h>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

class Bit {
	private:
		vector<int> tree;		
		int N;
	public:
		Bit(int _N = 0)	{
			N = _N+1;
			tree.resize(N,0);
		}

		~Bit() { }

		void updateFreq(int ind, int value) {
			ind++;
			while(ind<=N)	{
				tree[ind] += value;
				ind += (ind & -ind);
			}
		}
		
		int getFreq(int ind) {
			int sum=0;
			while(ind > 0) {		
				sum += tree[ind];
				ind &= ind - 1;
			}
			return sum;
		}

		int getFreq(int ind1, int ind2) {	
			if (ind1>ind2) return 0;
			return getFreq(ind2) - getFreq(ind1);
		}

};

pair<int, int> a[30010];
pair<pair<int, int>, pair<int, int> > q[200010];
int r[200010];
int main() {
	int n;
	scanf("%d", &n);
	Bit b(n);
	for (int i = 0; i < n; i++) {
		scanf("%d", &a[i].first);
		a[i].second = i;
	}
	sort(a, a+n);
	reverse(a, a+n);
	int nq;
	scanf("%d", &nq);
	for (int i = 0; i < nq; i++) {
		scanf("%d %d %d", &q[i].second.first, &q[i].second.second, &q[i].first.first);
		q[i].first.second = i;
	}
	sort(q, q+nq);
	reverse(q, q+nq);

	int atual = 0;
	for (int i = 0; i < nq; i++) {
		while (atual < n && a[atual].first > q[i].first.first) {
			b.updateFreq(a[atual].second, 1);
			atual++;
		}
		r[q[i].first.second] = b.getFreq(q[i].second.first - 1, q[i].second.second);
	}
	for (int i = 0; i < nq; i++)
		printf("%d\n", r[i]);
	return 0;
}
