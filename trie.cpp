char buf[1000010];
int main() {
    int _42;
    scanf("%d", &_42);
    fu(_41, _42) {
        printf("Case #%d:", _41+1);
				// THIS IS THE TRIE
				// child[26*cur + 'h'] is the child node corresponding
				// to following a 'h' node from cur.
        vi child(26*1000010, -1);
        int nchild = 1;
        int N;
        scanf("%d", &N);
        int ans = 0;
        while (N--) {
            scanf(" %s", buf);
            int cur = 0;
            int newnodes = 0;
            for (int i = 0; buf[i]; ++i) {
                int c = buf[i] - 'a';
                if (child[26*cur + c] == -1) {
                        child[26*cur + c] = nchild++;
                        newnodes++;
                }
                cur = child[26*cur + c];
            }
            int t = strlen(buf) - newnodes;
            if (newnodes == 0) ans += t;
            else ans += t + 1;
        }
        printf(" %d\n", ans);
    }
    return 0;
}
