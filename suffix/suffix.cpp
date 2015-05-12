struct suffix {
	vi ord;
	int N;
	int l;
	
	vi ind;
	template<typename T>
	void doit(int *V, vector<T>& order) {
		sort(all(ind), [&](int a, int b) {
				return order[a] < order[b];
				});
		int cur = 0;
		fu(i, N) {
			if (i && order[ind[i]] != order[ind[i-1]]) cur++;
			V[ind[i]] = cur;
		}
	}
	// Runs in O(n log^2 n)
	suffix(string s) {
		N = sz(s);
		ind.resize(N);
		iota(all(ind), 0);
		l = 33 - __builtin_clz(N-1);
		if (N == 1) l = 1;
		ord.resize(N*l);
		vector<char> V(N);
		fu(i, N) V[i] = s[i];
		doit(ord.data(), V);
		FU(j, 1, l) {
			vector<pair<int, int>> V2(N);
			fu(i, N) {
				V2[i] = {ord[i + (j-1)*N], -1};
				int ii = i + (1 << (j-1));
				if (ii < N) V2[i].second = ord[ii + (j-1)*N];
			}
			doit(ord.data() + j*N, V2);
		}
	}
	// computes the longest common preffix of the suffixes
	// that start at a and b. Does it in O(log N).
	int lcp(int a, int b) {
		int ans = 0;
		fd(j, l) {
			if (a >= N || b >= N) break;
			if (ord[a + j*N] == ord[b + j*N]) {
				ans += 1 << j; a += 1 << j; b += 1 << j;
			}
		}
		return ans;
	}
	// Return the order of the indices of the suffixes
	// Does it in O(N)
	vi order() {
		vi ans(N);
		int j = l-1;
		fu(i, N) ans[ord[i + j*N]] = i;
		return ans;
	}
};

