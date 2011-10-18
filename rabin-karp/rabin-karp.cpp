
const int RK_PRIME = 5800079;
// start: 4dba1d328127e8e610404a2eefc54322
vector<int> rabin_karp(const string &s, const string &pat, bool all=true) {
	vector<int> ret;
	int szPat = pat.size(), mult = 1, hp = 0, hs = 0;
	if (szPat > s.size()) return ret;
	for (int i=0; i < szPat; i++) {
		hp = (256 * hp + pat[i]) % RK_PRIME;
		if (i) mult = (256 * mult) % RK_PRIME;
	}
	for (int i=0; i < szPat; i++) hs = (256 * hs + s[i]) % RK_PRIME;
	if (hs == hp && strncmp(s.c_str(), pat.c_str(), szPat) == 0) {
		ret.push_back(0);
		if (!all) return ret;
	}
	for (int i=szPat; i < (int)s.size(); i++) {
		hs = (hs - mult * s[i - szPat]) % RK_PRIME;
		if (hs < 0) hs += RK_PRIME;
		hs = (256 * hs + s[i]) % RK_PRIME;
		if (hs == hp && strncmp(s.c_str()+i-szPat+1, pat.c_str(), szPat) == 0) {
			ret.push_back( i - szPat + 1 );
			if (!all) return ret;
		}
	}
	return ret;
}

