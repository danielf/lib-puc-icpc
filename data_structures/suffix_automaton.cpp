// Assumes everything is a-z!
// Easy to change otherwise
struct suffix_automaton {
	vector<array<int, 26>> next; // change here
	vi len;
	vi fpos;
	vi link;
	string s;
	vb isfinal;
	int last = 0;
	int create_node() {
		next.push_back(array<int, 26>()); // change here
		fill(all(next.back()), -1);
		len.push_back(0);
		fpos.push_back(-1);
		link.push_back(-1);
		return link.size() - 1;
	}

	void add_element(int value) {
		int newnode = create_node();
		len[newnode] = len[last] + 1;
		fpos[newnode] = len[last];

		int p = last;
		last = newnode;
		for (; p >= 0 && next[p][value] == -1; p = link[p])
			next[p][value] = newnode;
		if (p == -1) {
			link[newnode] = 0;
			return;
		}

		int q = next[p][value];
		if (len[q] == len[p] + 1) {
			link[newnode] = q;
			return;
		}
		int qq = create_node();
		next[qq] = next[q];
		len[qq] = len[p] + 1;
		fpos[qq] = fpos[q];
		link[qq] = link[q];
		for (; p >= 0 && next[p][value] == q; p = link[p])
			next[p][value] = qq;
		link[q] = qq;
		link[newnode] = qq;
	}

	suffix_automaton(string s) {
		this->s = s;
		const int N = s.size();
		next.reserve(N+1);
		len.reserve(N+1);
		link.reserve(N+1);
		fpos.reserve(N+1);
		create_node();
		for (char c : s) add_element(c - 'a'); // change here
		isfinal.assign(fpos.size(), false);
		for (int p = last; p != -1; p = link[p])
			isfinal[p] = true;
	}

	int rep(const string& pat) {
		int cur = 0;
		for (char c : pat) {
			int cc = c - 'a';
			if (next[cur][cc] == -1) return -1; // string is not substring
			cur = next[cur][cc];
		}
		return cur;
	}
};
