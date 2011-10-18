#include <vector>

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
			return getFreq(ind2) - getFreq(ind1);
		}

};

int main()
{
	Bit bit(4);
	bit.updateFreq(0,1); bit.updateFreq(1,1);
	bit.updateFreq(2,2); bit.updateFreq(3,0);

	printf("From 0 to 2 = %d\n", bit.getFreq(0,3));

}
