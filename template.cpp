#include <bits/stdc++.h>
using namespace std;

#define all(x) (x).begin(), (x).end()

#define TRACE(x) x
#define WATCH(x) TRACE(cout << #x" = " << x << endl)
#define PRINT(x...) TRACE(printf(x))
#define WATCHR(a, b) TRACE(for (auto c=a; c!=b;) cout << *(c++) << " "; cout << endl)
#define WATCHC(V) TRACE({cout << #V" = "; WATCHR(V.begin(), V.end());})

#define FU(i, a, b) for (auto i = a; i < b; ++i)
#define fu(i, b) FU(i, 0, b)
#define FD(i, a, b) for (auto i = (b) - 1; i >= a; --i)
#define fd(i, b) FD(i, 0, b)

using ll = long long;
using vi = vector<int>;
using vvi = vector<vi>;
using vll = vector<ll>;
using vvll = vector<vll>;
using vd = vector<double>;
using vb = vector<bool>;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

ll mod(ll a, ll b) {
  return ((a%b)+b)%b;
}

int cmp(double x, double y = 0, double tol = 1.e-7) {
	return (x <= y + tol) ? (x + tol < y) ? -1 : 0 : 1;
}
