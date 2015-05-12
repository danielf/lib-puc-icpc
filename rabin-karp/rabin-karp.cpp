const int RK_PRIME = 5800079;
// in case there is TLE

// all == true if want indices of all occurences of pat in s
// runs in O(N) but can be wrong with small probability!
//
// In case of WA, try changing the RK_PRIME to another prime number and rerunning
// try this a few times before giving up :)
vector<int> rabin_karp(const string &s, const string &pat, bool all=true) {
	vector<int> ret;
	int szPat = pat.size(), mult = 1, hp = 0, hs = 0;
	if (szPat > s.size()) return ret;
	for (int i=0; i < szPat; i++) {
		hp = (256 * hp + pat[i]) % RK_PRIME;
		if (i) mult = (256 * mult) % RK_PRIME;
	}
	for (int i=0; i < szPat; i++) hs = (256 * hs + s[i]) % RK_PRIME;
	if (hs == hp) {
		ret.push_back(0);
		if (!all) return ret;
	}
	for (int i=szPat; i < (int)s.size(); i++) {
		hs = (hs - mult * s[i - szPat]) % RK_PRIME;
		if (hs < 0) hs += RK_PRIME;
		hs = (256 * hs + s[i]) % RK_PRIME;
		if (hs == hp) {
			ret.push_back( i - szPat + 1 );
			if (!all) return ret;
		}
	}
	return ret;
}

