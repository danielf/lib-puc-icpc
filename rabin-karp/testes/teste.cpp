#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

#include "../rabin-karp.cpp"

string load_text(const string& filename) {
	ifstream infile(filename.c_str());
	if (!infile) 
		throw runtime_error("Unable to open file");

	string result;
	static const int buffer_size = 1 << 13;
	char buffer[buffer_size];
	while (infile) {
		infile.read(buffer, buffer_size);
		result.append(buffer,infile.gcount());
	}

	return result;
}

char buf1[1010], buf2[1010];
int main() {
	string haystack = load_text("mobydick.txt");
	//string needle = "devious-cruising Rachel";
	string needle = "Ishmael";
	vector<int> ret = rabin_karp(haystack, needle);
	if (!ret.size()) printf("-1\n");
	else {
		printf("%d", ret[0]);
		for (int i=1; i < (int)ret.size(); i++) printf(" %d", ret[i]);
		printf("\n");
	}
	
	return 0;
}
