struct BIT {
	// all indices are 0-indexed
	vi tree;
	int N;
	BIT(int _N){
		N = _N+1;
		tree = vi(N, 0);
	}
	void update(int ind, int delta){
		ind++;
		while(ind<N) {
			tree[ind] += delta;
			ind += (ind & -ind);
		}
	}
	// returns sum of [0, ind], closed interval
	int get(int ind) {
		ind++;
		int sum=0;
		while(ind > 0) {
			sum += tree[ind];
			ind -= (ind & -ind);
		}
		return sum;
	}
	// returns sum of [ind1, ind2], closed interval
	int get(int ind1, int ind2) {
		if (ind1>ind2) return 0;
		return get(ind2) - get(ind1-1);
	}
};
