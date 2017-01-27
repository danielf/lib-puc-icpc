#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <inttypes.h>

#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

using namespace std;

#define TRACE(x...)
#define PRINT(x...) TRACE(printf(x))
#define WATCH(x) TRACE(cout << #x" = " << x << "\n")
#define FOR(i, a, b) for(int i=a;i<b;i++)
#define MSET(a, b) memset(a, b, sizeof(a))

#define INF 0x3f3f3f3f

struct RMQ
{
  vector<int> M;
  vector<int> R;
  int N,I,F;

  RMQ(vector<int>& _R) {
    R=_R;
    N=R.size();
    M.resize(8*N+10);
    _makeTree(1,0,N);
  }

  // Retorna o minimo no intervalo [a, b) em O(log N)
  int getMin(int a, int b) {
    I=a;F=b;
    return _find(1,0,N);
  }

  void update(int pos, int num) { // O(log N)
    R[pos] = num;
    I=pos;F=pos+1;
    _update(1, 0, N);
  }

  int _find(int node, int a, int b) { // O(log N)
    if (a >= I && b <= F) return M[node];
    if (a >= F || b <= I) return -1;
    int left = _find(2*node, a, (a+b)/2);
    int right = _find(2*node+1, (a+b)/2, b);
    if(left == -1 || right == -1) return max(left,right);
    if (R[left] <= R[right]) return left;
    return right;
  }

  void _makeTree(int node, int a, int b) { // O(4*N)
    if (a+1==b) {
      M[node] = a;
      return;
    }
    _makeTree(2*node, a, (a+b)/2);
    _makeTree(2*node+1, (a+b)/2, b);
    if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node];
    else M[node] = M[2*node+1];
  }

  void _update(int node, int a, int b) { // O(log N)
    if (a == I && b == F) {
      M[node] = a;
      return;
    }
    if (a >= F || b <= I) return;
    // Minimo entre o intervalo da esquerda e direita
    _update(2*node, a, (a+b)/2);
    _update(2*node+1, (a+b)/2, b);
    if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node];
    else M[node] = M[2*node+1];
  }
};

int main()
{
  vector<int> A(5);
  A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 3; A[4] = 4;
  RMQ tree(A);
  //tree.init(A);
  printf("Min between [0,1): %d\n",tree.getMin(0,1));
  printf("Min between [0,5): %d\n",tree.getMin(0,5));
  printf("Min between [3,5): %d\n",tree.getMin(3,5));
  return 0;
}
