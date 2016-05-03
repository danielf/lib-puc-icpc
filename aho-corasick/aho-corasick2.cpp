// start: a2221b6084725a15a0fcaf6a2f9487ae  - whole file
namespace aho_corasick {
// start: 8f81be54f204c0900675e37ff2d38f11  -
    const int SIGMA = 2;
    const int TOTL = 1e7 + 100;

    struct node {
        int link[SIGMA];
        int suff, dict, patt;
        node() { 
            suff = 0, dict = 0, patt = -1;
            memset(link, 0, sizeof(link));
        }
        // link[]: contains trie links + failure links
        // suff: link to longest proper suffix that exists in the trie
        // dict: link to longest suffix that exists in the dictionary
        // patt: index of this node's word in the dictionary
    };
// end

// start: 3f737f00a0c3a1bbb16b909b47f86e8f  -
    int tail = 1;
    vector<node> trie(TOTL);
    vector<string> patterns;
// end

// start: 9e765f7ab5603a69c71e9af388d5540a  -
    void add_pattern(string &s) {
        int loc = 0;
        for (char c : s) {
            int &nloc = trie[loc].link[c-'a'];
            if (!nloc) nloc = tail++;
            loc = nloc;
        }

        trie[loc].dict = loc;
        trie[loc].patt = patterns.size();
        patterns.push_back(s);
    }
// end

// start: c0641930c8920d56dc94b56222757e4d  -
    void calc_links() {
        queue<int> bfs({0});

        while (!bfs.empty()) {
            int loc = bfs.front(); bfs.pop();
            int fail = trie[loc].suff;

// 		start: 38fa27c06e7e84ed7ebed30d2e594f64  -
            if (!trie[loc].dict) 
                trie[loc].dict = trie[fail].dict;

            for (int c = 0; c < SIGMA; c++) {
                int &succ = trie[loc].link[c];
                if (succ) {
                    trie[succ].suff = loc ? trie[fail].link[c] : 0;
                    bfs.push(succ);
                } else succ = trie[fail].link[c];
            }
// 		end
        }
    }
// end

// start: acbb7c621c282c53025ea4c2a389e892  -
    void match(string &s, vector<bool> &matches) {
        int loc = 0;

        for (char c : s) {
            loc = trie[loc].link[c-'a'];

            for (int dm = trie[loc].dict; dm; dm = trie[trie[dm].suff].dict) {
                if (matches[trie[dm].patt]) break;
                matches[trie[dm].patt] = true;
            }
        }
    }
// end
}
// end
