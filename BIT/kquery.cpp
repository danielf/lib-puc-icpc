#include <vector>
#include <algorithm>
#include <utility>
#include <stdio.h>

#define FOR(i,a,b) for(int i=(a);i<(b);++i)

using namespace std;

class Bit
{
	private:
		vector<int> tree;		
		int N;

	public:
		Bit(int _N){
			N = _N+1;
			tree.resize(N,0);
		}	
		
		void update(int ind, int value){
			ind++;
			while(ind<N)
			{
				tree[ind] += value;
				ind += (ind & -ind);
			}	
		}
		
		int get(int ind){
			int sum=0;
			while(ind > 0)
			{		
				sum += tree[ind];
				ind -= (ind & -ind);
			}
			return sum;
		}

		int get(int ind1, int ind2)	{	
			if (ind1>=ind2) return 0;
			return get(ind2) - get(ind1);
		}

};

pair<int,int> A[30000+10];
pair<int, pair<int, pair<int,int> > >queries[200000+10];
int N, Q;

int main()
{
	scanf("%d",&N);
	FOR(i,0,N){
		 int a;
		 scanf("%d",&a);
		 A[i] = make_pair(a,i);
	}
	scanf("%d",&Q);
	FOR(i,0,Q) {
		int a,b,c;
		scanf("%d %d %d",&a,&b,&c);a--;b--;
		queries[i] = make_pair(c,make_pair(i,make_pair(a,b)));
	}
	sort(queries,queries+Q);
	sort(A,A+N);

	Bit bit(N+1);
	int i=0;
	vector<int> ans(Q);
	FOR(q,0,Q)
	{	
		while(i<N && A[i].first<=queries[q].first)
		{
			bit.update(A[i].second,+1);
			i++;
		}
		int x = queries[q].second.second.first;
		int y = queries[q].second.second.second;
		ans[queries[q].second.first] = y-x+1-bit.get(x,y+1);
	}
	FOR(i,0,Q) printf("%d\n",ans[i]);
	return 0;
}
