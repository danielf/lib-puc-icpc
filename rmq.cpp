#include <vector>

struct RMQ {
	vector<int> M, R;
	int N, I, F;

	RMQ(vector<int>& _R) { // start: 1473180946d7d3ad5bf7110c95ee029a
		R = _R; N = R.size();
		M.resize(8 * N + 10);
		_makeTree(1, 0, N);
	}

	// Retorna o minimo no intervalo [a, b) em O(log N)
	int getMin(int a, int b) { // start: 63dcd3c5075c9f00fa109b4f805a1bdc
		I = a; F = b;
		return _find(1, 0, N);
	}

	// start: 80e27103d7c24c641c8fec834873ff9e
	void update(int pos, int num) { // O(log N)
		R[pos] = num;
		I = pos; F = pos+1;
		_update(1, 0, N);
	}

	// start: e25f0641b3986c461fdc8c7b893dc49c
	int _find(int node, int a, int b) { // O(log N)
		if (a >= I && b <= F) return M[node];
		if (a >= F || b <= I) return -1;
		int left = _find(2*node, a, (a+b)/2);
		int right = _find(2*node+1, (a+b)/2, b);
		if (left == -1 || right == -1) return max(left, right);
		if (R[left] <= R[right]) return left; // (*)
		return right;
	}

	// start: 214ab15f613232c2d8ebc7d6f22cddb7
	void _makeTree(int node, int a, int b) { // O(4*N)
		if (a+1 == b) { M[node] = a; return; }
		_makeTree(2*node, a, (a+b)/2);
		_makeTree(2*node+1, (a+b)/2, b);
		if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node]; // (*)
		else M[node] = M[2*node+1];
	}

	// start: 7f855e9e062ac6aec20c68a323d68ab8
	void _update(int node, int a, int b) { // O(log N)
		if (a == I && b == F) { M[node] = a; return; }
		if (a >= F || b <= I) return;
		_update(2*node, a, (a+b)/2);
		_update(2*node+1, (a+b)/2, b);
		if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node]; // (*)
		else M[node] = M[2*node+1];
	}
	// (*) trocar <= por >= para MAX query
};
