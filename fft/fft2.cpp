// start: 2935b884c0096beabcb823a757fdb824  - whole file
namespace fft {
		typedef complex<double> cnum;
    int deg;
    vector<int> rev;
// start: 29041032f735591da4bfb49d1ec0aa11  -
    void set_degree(int _deg) {
        assert(__builtin_popcount(_deg) == 1);
        deg = _deg;
        rev.resize(deg);
        for (int i = 1, j = 0; i < deg; i++) {
            int bit = deg / 2;
            for (; j >= bit; bit /= 2)
                j -= bit;
            j += bit;
            rev[i] = j;
        }
    }
// end
// start: 20685bb8b7d6e492e8352bd47c4d8e05  -
    void transform(vector<cnum> &poly, bool invert) {
        if(deg != poly.size()) set_degree(poly.size());
        for (int i = 1; i < deg; i++)
            if(rev[i] > i)
                swap(poly[i], poly[rev[i]]);
        for (int len = 2; len <= deg; len *= 2) {
            double ang = 2 * M_PI / len * (invert ? -1 : 1);
            cnum base = { cos(ang), sin(ang) };
// 		start: 8d920ac089ce3baee8de11c9eac96b65  -
            for (int i = 0; i < deg; i += len) {
                cnum w = {1, 0};
                for (int j = 0; j < len / 2; j++) {
                    cnum u = poly[i+j];
                    cnum v = w * poly[i+j+len/2];
                    poly[i+j] = u + v;
                    poly[i+j+len/2] = u - v;
                    w = w * base;
                }
            }
// 		end
        }
        if(invert) {
            for (int i = 0; i < deg; i++)
                poly[i] = poly[i] / double(deg);
        }
    }
// end
};
// end
