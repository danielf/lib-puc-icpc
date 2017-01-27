// start: f68565956896cd43bc1e7066f456c061  - whole file
template<typename T> struct BIT{
  int S;
  vector<T> v;

// start: e1ebba0510bdac19ab1e7d3a2bc59902  -
  BIT<T>(int _S){
    S = _S;
    v.resize(S+1);
  }

  void update(int i, T k){
    for(i++; i<=S; i+=i&-i)
      v[i] = v[i] + k;
  }
// end

// start: 885ec1a91a9e8af4e1476f3bb24cdc86  -
  T read(int i){
    T sum = 0;
    for(i++; i; i-=i&-i)
      sum = sum + v[i];
    return sum;
  }

  T read(int l, int r){
    return read(r) - read(l-1);
  }
// end
};
// end
