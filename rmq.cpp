#include <vector>

struct RMQ {
	vector<int> M, R;
	int N, I, F;

	RMQ(vector<int>& _R) {
		R = _R; N = R.size();
		M.resize(8 * N + 10);
		_makeTree(1, 0, N);
	}

	// Returns minimum of interval [a,b) in O(log N)
	int getMin(int a, int b) {
		I = a; F = b;
		return _find(1, 0, N);
	}
	// makes V[pos] = num in O(log N)
	void update(int pos, int num) {
		R[pos] = num;
		I = pos; F = pos+1;
		_update(1, 0, N);
	}
	int _find(int node, int a, int b) {
		if (a >= I && b <= F) return M[node];
		if (a >= F || b <= I) return -1;
		int left = _find(2*node, a, (a+b)/2);
		int right = _find(2*node+1, (a+b)/2, b);
		if (left == -1 || right == -1) return max(left, right);
		if (R[left] <= R[right]) return left; // (*)
		return right;
	}
	void _makeTree(int node, int a, int b) {
		if (a+1 == b) { M[node] = a; return; }
		_makeTree(2*node, a, (a+b)/2);
		_makeTree(2*node+1, (a+b)/2, b);
		if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node]; // (*)
		else M[node] = M[2*node+1];
	}
	void _update(int node, int a, int b) {
		if (a == I && b == F) { M[node] = a; return; }
		if (a >= F || b <= I) return;
		_update(2*node, a, (a+b)/2);
		_update(2*node+1, (a+b)/2, b);
		if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node]; // (*)
		else M[node] = M[2*node+1];
	}
	// (*) change <= to >= to have MAX query instead
};
