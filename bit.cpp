#include <vector>

struct BIT { // start: 830e3dd21ec48035fdd85c1a6225576a
	vector<int> tree;		
	int N;

	BIT(int _N=0) {
		N = _N+1;
		tree.resize(N,0);
	}	
	
	void update(int ind, int value) { // O(log n)
		ind++;
		while (ind < N) {
			tree[ind] += value;
			ind += (ind & -ind);
		}	
	}
	
	int get(int ind) { // O(log n)
		int sum=0;
		while (ind > 0) {		
			sum += tree[ind];
			ind -= (ind & -ind);
		}
		return sum;
	}

	int get(int ind1, int ind2)	{ // O(log n)
		if (ind1 >= ind2) return 0;
		return get(ind2) - get(ind1);
	}
};
