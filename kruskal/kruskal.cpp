// Union-find
int P[10010], H[10010];
void init(int N) {
  fu(i, N) {P[i] = i; H[i] = 0;}
}
int rep(int i) {
  if (P[i] != i) P[i] = rep(P[i]);
  return P[i];
}
bool unio(int a, int b) { // returns if union was successful
  a = rep(a); b = rep(b);
  if (a == b) return false;
  if (H[a] < H[b]) P[a] = b;
  else P[b] = a;
  if (H[a] == H[b]) H[a]++;
  return true;
}
// kruskal
pair<int, pair<int, int> > edges[1000010]; // (w, (i,j))
int kruskal(int N, int M) {
  init(N);
  int ans = 0;
  sort(edges, edges+M);
  fu(i, M) if (unio(edges[i].second.first, edges[i].second.second))
    ans += edges[i].first;
  return ans;
}
