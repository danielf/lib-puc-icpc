#include <stdio.h>
#include <vector>
#include <algorithm>
#include <inttypes.h>

using namespace std;

#define MAGIC 1000000007

class Bit
{
  private:
    vector<long long> tree;
    int N;

  public:
    Bit(int _N)
    {
      N = _N+1;
      tree.resize(N,0);
    }

    ~Bit()
    {

    }

    void updateFreq(int _ind, int value)
    {
      long long ind = _ind+1;
      while(ind<=N)
      {
        tree[ind] += value;
        tree[ind] %= MAGIC;
        ind += (ind & -ind);
      }
    }

    long long getFreq(int ind)
    {
      long long sum=0;
      while(ind > 0)
      {
        sum += tree[ind];
        sum %= MAGIC;
        ind -= (ind & -ind);
      }
      return sum;
    }

    long long getFreq(int ind1, int ind2)
    {
      if (ind1>=ind2) return 0;
      return (getFreq(ind2) - getFreq(ind1))%MAGIC;
    }

};

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define value first
#define idx second

pair<long long,long long> A[500000+10];
long long B[500000+10];

long long T, N, M, X,Y,Z;

int main()
{
  scanf("%lld",&T);
  FOR(caseId,0,T)
  {

    //generating
    scanf("%lld %lld %lld %lld %lld",&N,&M,&X,&Y,&Z);
    FOR(i,0,M) {
      scanf("%lld",&B[i]);
    }
    X %= Z;
    Y %= Z;
    FOR(i,0,N){
      A[i].value = B[i%M];
      A[i].idx = i;
      B[i%M]= (X*(B[i%M]%Z) + Y*((i+1)%Z))%Z;
    }
    //finished

    sort(A,A+N);
    Bit bit(N);

    bit.updateFreq(A[0].idx,1);
    long long lastEqual=0;
    FOR(i,1,N)
    {
      long long total = (bit.getFreq(A[i].idx)+1)%MAGIC;
      if (A[i].value == A[i-1].value){
        for(int j=i-1; j>=lastEqual; --j){
          total -= bit.getFreq(A[j].idx,A[j].idx+1);
          total %= MAGIC;
        }
      }else
        lastEqual = i;
      total %= MAGIC;
      bit.updateFreq(A[i].idx,total);
    }
    long long ans=0;
    FOR(i,1,N+1) {
      ans += bit.getFreq(i-1,i);
      ans %= MAGIC;
    }
    printf("Case #%lld: %lld\n",1+caseId,ans);
  }

}
