#include <vector>

struct BIT {
	vector<int> tree;		
	int N;

	BIT(int _N=0) {
		N = _N+1;
		tree.resize(N,0);
	}	
	// makes V[ind] += value (value can be negative)
	void update(int ind, int value) { // O(log n)
		ind++;
		while (ind < N) {
			tree[ind] += value;
			ind += (ind & -ind);
		}	
	}
	// Gets the sum of first ind elements
	int get(int ind) { // O(log n)
		int sum=0;
		while (ind > 0) {		
			sum += tree[ind];
			ind -= (ind & -ind);
		}
		return sum;
	}
	// Gets sum of [ind1, ind2)
	int get(int ind1, int ind2)	{ // O(log n)
		if (ind1 >= ind2) return 0;
		return get(ind2) - get(ind1);
	}
};
