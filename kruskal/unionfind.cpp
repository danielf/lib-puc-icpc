struct UF {
  vi p, rank;
  UF(int n) {
    p = vi(n);
    rank = vi(n, 0);
    fu(i, n) p[i] = i;
  }
  int rep(int v) {
    if (p[v] != v) p[v] = rep(p[v]);
    return p[v];
  }
  bool unio(int a, int b) { // returns false if a and b are already joined
    a = rep(a); b = rep(b);
    if (a == b) return false;
    if (rank[a] < rank[b]) p[a] = b;
    else p[b] = a;
    if (rank[a] == rank[b]) rank[a]++;
    return true;
  }
};
