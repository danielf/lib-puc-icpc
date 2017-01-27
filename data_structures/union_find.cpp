// start: 2feb924109507119d3da1584e7e1c7d2  -
struct union_find {
  vector<int> P, R, S;

//   start: fe7af7970a172b3ba0c164de046b4255  -
  union_find (int N) {
    P.resize(N), R.resize(N, 0), S.resize(N, 1);
    for (int i = 0; i < N; i++)
      P[i] = i;
  }

  int rep(int i) {
    if(P[i] != i) P[i] = rep(P[i]);
    return P[i];
  }
//   end

//   start: 953d810382f20fe63d56d0468f8db407  -
  bool unio(int a, int b) {
    a = rep(a), b = rep(b);
    if(a == b) return false;
    if(R[a] < R[b]) swap(a, b);
    P[b] = a;
    S[a] += S[b];
    if(R[a] == R[b]) R[a]++;
    return true;
  }
//   end
};
// end
