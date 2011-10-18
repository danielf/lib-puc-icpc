template <class T>
struct index_lt {
	T& v;
	index_lt(T& v): v(v) {}
	inline bool operator ()(int i, int j) {
		return (v[i] != v[j]) ? (v[i] < v[j]) : (i < j);
	}
};
template <class T> index_lt<T> make_index_lt(T& v) { return index_lt<T>(v); }

bool cmp_eq(double x, double y) { return cmp(x, y) == 0; }
bool cmp_lt(double x, double y) { return cmp(x, y) < 0; }

int safe_gets(char*& s) { // depois de usar, free(s);
	return scanf("%a[^\r\n]%*[\r\n]", &s);
}
