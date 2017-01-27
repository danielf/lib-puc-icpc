// start: e230b9ce0bec166514dbd36acbef397d  - whole file
template<typename T> struct kmp {
  int M;
  vector<T> needle;
  vector<int> succ;

// start: 45ca30a8137ea3c18bf3a9403b6bcb7c  -
  kmp(vector<T> _needle) {
    needle = _needle;
    M = needle.size();

    succ.resize(M + 1);
    succ[0] = -1, succ[1] = 0;

    int cur = 0;
//   start: d44217b23c53ea5bcdb0b61a260b59e0  -
    for (int i = 2; i <= M; ) {
      if (needle[i-1] == needle[cur]) succ[i++] = ++cur;
      else if (cur) cur = succ[cur];
      else succ[i++] = 0;
    }
//   end
  }
// end

// start: 23dc1986f9ad2d0dee8fef0be30d11da  -
  vector<bool> find(vector<T> &haystack) {
    int N = haystack.size(), i = 0;
    vector<bool> res(N);

    for (int m = 0; m + i < N; ) {
//   start: 771eb9d8767922136b1954939a8cdda6  -
      if (i < M && needle[i] == haystack[m + i]) {
        if (i == M - 1) res[m] = true;
        i++;
      } else if (succ[i] != -1) {
        m = m + i - succ[i];
        i = succ[i];
      } else {
        i = 0;
        m++;
      }
//   end
    }

    return res;
  }
// end
};
// end
