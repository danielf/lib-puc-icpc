const char* rank_names = "**23456789TJQKA";  // CUIDADO (BUG): 23456 < 34567 <
const char* suit_names = "CDHS";             // < ... < 9TJQK < _A2345_ < TJQKA

struct card { // start: eee8007cf8011375427e553dfbd5316e
	int rank, suit;
	int read() {
		char ch[2];
		if (scanf(" %c%c", &ch[0], &ch[1]) == EOF) return 0;
		for (rank = 0; rank_names[rank] != ch[0]; rank++);
		for (suit = 0; suit_names[suit] != ch[1]; suit++);
		return 1;
	}
	void print() { printf("%c%c", rank_names[rank], suit_names[suit]); }
};

struct freq_lt { // start: 3042c3c9b5284f136f0c45df53a4dfcb
	int* freq;
	freq_lt(int* freq): freq(freq) {}
	bool operator ()(const card A, const card B) const {
		if (int t = freq[A.rank] - freq[B.rank]) return t > 0;
		else return A.rank > B.rank;
	}
};

struct hand { // start: c3e514755a06760a66ae35a1b7d79797
	card C[5];
	int type() {
		int freq[15]; memset(freq, 0, sizeof(freq));

		sort(C, C+5, freq_lt(freq));
		bool flush = true, straight = true;
		for (int i = 0; i < 5; i++) {
			if (i && C[i].suit != C[i-1].suit) flush = false;
			if (i && !(C[i].rank == 5 && C[i-1].rank == 14) && \
			    C[i].rank != C[i-1].rank - 1) straight = false;
			freq[C[i].rank]++;
		}
		sort(C, C+5, freq_lt(freq));

		int kind[5]; memset(kind, 0, sizeof(kind));
		for (int i = 2; i <= 14; i++) kind[freq[i]]++;
		if (straight && flush) return 8;
		else if (kind[4]) return 7;
		else if (kind[3] && kind[2]) return 6;
		else if (flush) return 5;
		else if (straight) return 4;
		else if (kind[3]) return 3;
		else return kind[2];
	}
	bool operator <(hand H) {
		if (int t = type() - H.type()) return t < 0;
		for (int i = 0; i < 5; i++)
			if (int t = C[i].rank - H.C[i].rank) return t < 0;
		return false;
	}
};
