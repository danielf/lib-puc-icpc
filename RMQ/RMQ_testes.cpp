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

        RMQ(vector<int>& _R){
            R=_R;
            N=R.size();
            M.resize(8*N+10);
            _makeTree(1,0,N);
        }
        int getMin(int a, int b){
            I=a;F=b;
            return _find(1,0,N);
        }
        void update(int pos, int num) {
          R[pos] = num;
          I=pos;F=pos+1;
          _update(1, 0, N);
        }

        // O(log N)
        int _find(int node, int a, int b) {
            if (a >= I && b <= F) return M[node];
            if (a >= F || b <= I) return -1;
            int left = _find(2*node, a, (a+b)/2);
            int right = _find(2*node+1, (a+b)/2, b);
            if(left == -1 || right == -1) return max(left,right);
            if (R[left] <= R[right]) return left;
            return right;
        }
        // O(4*N)
        void _makeTree(int node, int a, int b) {
            if (a+1==b) {
                M[node] = a;
                return;
            }
            _makeTree(2*node, a, (a+b)/2);
            _makeTree(2*node+1, (a+b)/2, b);
            if (R[M[2*node]] <= R[M[2*node+1]]) M[node] = M[2*node];
            else M[node] = M[2*node+1];
        }
        // O(log N)
        void _update(int node, int a, int b) {
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

        // ESTA FUNCAO EXISTE SOMENTE PARA TESTE
        int bruteforce(int a, int b) {
          int ans = a;
          FOR (i, a, b) {
            if (R[i] < R[ans]) ans = i;
          }
          return ans;
        }

};

int main()
{
  int N, _42;
  char nome[50];
  scanf(" %d", &_42);
  while (_42--) {
    scanf(" %s", nome);
    scanf(" %d", &N);
    vector<int> V;
    FOR (i, 0, N) {
      int aux;
      scanf(" %d", &aux);
      V.push_back(aux);
    }
    RMQ tree(V);
    printf("Testando %s\n", nome);
    FOR (i, 0, N) {
      FOR (j, i+1, N+1) {
        tree.update(i, rand()%45);
        if (tree.getMin(i, j) != tree.bruteforce(i, j)) {
          printf("ERRO %d %d %d\n", N, i, j);
        }
      }
    }
  }

  /*
  vector<int> A(5);
  A[0] = 1; A[1] = 2; A[2] = 3; A[3] = 3; A[4] = 4;
  RMQ tree;
  tree.init(A);
  printf("Min between [0,1): %d\n",tree.getMin(0,1));
  printf("Min between [0,5): %d\n",tree.getMin(0,5));
  printf("Min between [3,5): %d\n",tree.getMin(3,5));
  */
  return 0;
}
