#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define REP(i, n) FOR(i, 0, n)
#define TRACE(x) cout << #x << " = " << x << endl
#define _ << " _ " <<

typedef long long llint;

const int MAXN = 1<<20;
const int mod = 1e9;

const int off = 1<<20;

int add(int a, int b) {
  return a+b >= mod ? a+b-mod : a+b;
}

int sub(int a, int b) {
  return a >= b ? a-b : a-b+mod;
}

int mul(int a, int b) {
  return (llint(a)*b)%mod;
}

int sumi(int n) { // 0 + 1 + ... + n-1
  return (llint(n) * (n-1) / 2) % mod;
}

int sumij(int lo, int hi) { // lo + .. + hi-1
  return sub(sumi(hi), sumi(lo));
}

struct data {
  int k, l, sum, cnt, cnt1;
  bool set;
  int val;
  int lo, hi;
  int len1;

  void setval(int v) {
    set = true;
    val = v;
    k = l = 0;
    sum = mul(hi-lo, v);
    if (v%2 == 1) {
      cnt = hi - lo;
      cnt1 = len1;
    } else
      cnt = cnt1 = 0;
  }
  
  void addk(int ak) {
    k = add(k, ak);
    if (ak%2 == 1) {
      cnt -= cnt1;
      cnt1 = len1 - cnt1;
      cnt += cnt1;
    }
    sum = add(sum, mul(sumij(lo, hi), ak));
  }
  
  void addl(int al) {
    l = add(l, al);
    if (al%2 == 1) cnt = hi-lo - cnt, cnt1 = len1 - cnt1;
    sum = add(sum, mul(hi - lo, al));
  }

  void update(data &a, data &b) {
    sum = add(a.sum, b.sum);
    cnt = a.cnt + b.cnt;
    cnt1 = a.cnt1 + b.cnt1;
  }
} T[2*off];

void propagate(int i) {
  if (T[i].set) {
    T[i*2].setval(T[i].val);
    T[i*2+1].setval(T[i].val);
    T[i].set = false;
  }
  if (T[i].k) {
    T[i*2].addk(T[i].k);
    T[i*2+1].addk(T[i].k);
    T[i].k = 0;
  }
  if (T[i].l) {
    T[i*2].addl(T[i].l);
    T[i*2+1].addl(T[i].l);
    T[i].l = 0;
  }
}

void addkl(int i, int lo, int hi, int a, int b, int k, int l) {
  if (lo >= b || hi <= a) return;
  if (lo >= a && hi <= b) {
    T[i].addk(k);
    T[i].addl(l);
    return;
  }

  propagate(i);
  addkl(i*2, lo, (lo + hi) / 2, a, b, k, l);
  addkl(i*2+1, (lo + hi) / 2, hi, a, b, k, l);
  T[i].update(T[i*2], T[i*2+1]);
}

void setval(int i, int lo, int hi, int a, int b, int val) {
  if (lo >= b || hi <= a) return;
  if (lo >= a && hi <= b) {
    T[i].setval(val);
    return;
  }

  propagate(i);
  setval(i*2, lo, (lo + hi) / 2, a, b, val);
  setval(i*2+1, (lo + hi) / 2, hi, a, b, val);
  T[i].update(T[i*2], T[i*2+1]);
}

int getsum(int i, int lo, int hi, int a, int b) {
  if (lo >= b || hi <= a) return 0;
  if (lo >= a && hi <= b) return T[i].sum;
  propagate(i);
  int ans = 0;
  ans = add(ans, getsum(i*2, lo, (lo + hi) / 2, a, b));
  ans = add(ans, getsum(i*2+1, (lo + hi) / 2, hi, a, b));
  return ans;
}

int getcnt(int i, int lo, int hi, int a, int b) {
  if (lo >= b || hi <= a) return 0;
  if (lo >= a && hi <= b) return T[i].cnt;
  propagate(i);
  int ans = 0;
  ans += getcnt(i*2, lo, (lo + hi) / 2, a, b);
  ans += getcnt(i*2+1, (lo + hi) / 2, hi, a, b);
  return ans;
}

int S[MAXN], O[MAXN];
int A[MAXN], B[MAXN];
int C[MAXN], D[MAXN];

void gen(int S1, int S2, llint XS, llint YS, llint ZS, int N, int mod, int add, int S[MAXN]) {
  S[0] = S1, S[1] = S2;
  FOR(i, 2, N) 
    S[i] = ((llint(XS) * S[i-2] + llint(YS) * S[i-1] + ZS) % mod) + add;
}

int main(void) {
  int TC;
  scanf("%d", &TC);
  for (int tp = 1; tp <= TC; ++tp) {
    int N, M;
    int S1, S2, XS, YS, ZS;
    int O1, O2, XO, YO, ZO;
    int A1, A2, XA, YA, ZA;
    int B1, B2, XB, YB, ZB;
    int C1, C2, XC, YC, ZC;
    int D1, D2, XD, YD, ZD;

    scanf("%d %d", &N, &M);
    scanf("%d %d %d %d %d", &S1, &S2, &XS, &YS, &ZS);
    scanf("%d %d %d %d %d", &O1, &O2, &XO, &YO, &ZO);
    scanf("%d %d %d %d %d", &A1, &A2, &XA, &YA, &ZA);
    scanf("%d %d %d %d %d", &B1, &B2, &XB, &YB, &ZB);
    scanf("%d %d %d %d %d", &C1, &C2, &XC, &YC, &ZC);
    scanf("%d %d %d %d %d", &D1, &D2, &XD, &YD, &ZD);
    
    gen(S1, S2, XS, YS, ZS, N, mod, 0, S);
    gen(O1, O2, XO, YO, ZO, M, 4, 1, O);
    gen(A1, A2, XA, YA, ZA, M, N, 1, A);
    gen(B1, B2, XB, YB, ZB, M, N, 1, B);
    gen(C1, C2, XC, YC, ZC, M, mod, 0, C);
    gen(D1, D2, XD, YD, ZD, M, mod, 0, D);

    REP(i, 2*off) {
      T[i].k = T[i].l = T[i].cnt = T[i].cnt1 = T[i].sum = 0;
      T[i].lo = T[i].hi = T[i].len1 = T[i].val = 0;
      T[i].set = false;
    }

    FOR(i, off, 2*off) {
      T[i].lo = i-off;
      T[i].hi = T[i].lo + 1;
      T[i].len1 = (i-off) % 2;
    }
    for (int i = off-1; i > 0; --i) {
      T[i].lo = T[i*2].lo;
      T[i].hi = T[i*2+1].hi;
      T[i].len1 = T[i*2].len1 + T[i*2+1].len1;
    }

    REP(i, N) setval(1, 0, off, i, i+1, S[i]);
    REP(i, M) --A[i];

    int ans = 0;
    REP(i, M) {
      int qans = 0;
      int a = A[i];
      int b = A[i] + B[i];
      
      if (O[i] == 1) {
        int k = D[i];
        int l = sub(C[i], mul(k, a));

        qans = add(qans, mul(B[i], C[i]));
        qans = add(qans, mul(sumi(B[i]), D[i]));

        if (b <= N) addkl(1, 0, off, a, b, k, l); else {
          addkl(1, 0, off, a, N, k, l);
          l = add(C[i], mul(N - a, k));
          addkl(1, 0, off, 0, b%N, k, l);
        }
      }
     
      if (O[i] == 2) {
        if (b <= N) qans = add(qans, getsum(1, 0, off, a, b)); else {
          qans = add(qans, getsum(1, 0, off, a, N));
          qans = add(qans, getsum(1, 0, off, 0, b%N));
        }

        qans = add(qans, mul(B[i], C[i]));
        if (b <= N) setval(1, 0, off, a, b, C[i]); else {
          setval(1, 0, off, a, N, C[i]);
          setval(1, 0, off, 0, b%N, C[i]);
        }
      }

      if (O[i] == 3) {
        if (b <= N) qans = add(qans, getsum(1, 0, off, a, b)); else {
          qans = add(qans, getsum(1, 0, off, a, N));
          qans = add(qans, getsum(1, 0, off, 0, b%N));
        }
      }
      
      if (O[i] == 4) {
        if (b <= N) qans = add(qans, getcnt(1, 0, off, a, b)); else {
          qans = add(qans, getcnt(1, 0, off, a, N));
          qans = add(qans, getcnt(1, 0, off, 0, b%N));
        }
      }
			printf("%d - %d\n", O[i], qans);
      
      ans = add(ans, qans);
    }

    printf("Case #%d: ", tp);
    printf("%d\n", ans);
  }
  return 0;
}
