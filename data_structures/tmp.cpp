int main() {
	string s = "abcbbb";
	suffix_automaton SA(s);
	printf("%s:\n", s.c_str());
	fu(i, s.size()) FU(j, i+1, s.size() + 1) {
		string tmp = s.substr(i, j-i);
		int r = SA.rep(tmp);
		printf("\t%s - %d (%d)\n", tmp.c_str(), r, !!SA.isfinal[r]);
	}

	printf("%d\n", (int)SA.isfinal.size());
	printf("node link fpos len: (char, next) [*]\n");
	fu(i, SA.isfinal.size()) {
		printf("%4d %4d %4d %3d:", i, SA.link[i], SA.fpos[i], SA.len[i]);
		fu(j, 26) if (SA.next[i][j] != -1)
			printf(" (%c,%d)", j + 'a', SA.next[i][j]);
		if (SA.isfinal[i]) printf(" *");
		printf("\n");
	}
	return 0;
}
