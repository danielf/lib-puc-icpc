struct BIT {
	// all indices are 0-indexed
	vi tree;
	int N;
	BIT(int _N){
		N = _N+1;
		tree = vi(N, 0);
	}	
	void update(int ind, int value){
		ind++;
		while(ind<N) {
			tree[ind] += value;
			ind += (ind & -ind);
		}	
	}
	int get(int ind) {
		ind++;
		int sum=0;
		while(ind > 0) {
			sum += tree[ind];
			ind -= (ind & -ind);
		}
		return sum;
	}
	int get(int ind1, int ind2) {
		if (ind1>ind2) return 0;
		return get(ind2) - get(ind1-1);
	}
};
