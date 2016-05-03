// start: be95890887511431eb5c7d815bc1d3c5  - whole file
struct upper_envelope {
    typedef pair<ll, ll> pll;
    // key of map is a pll. first is slope, second is "break point"
    // it's increasing in both, by_slope controls which to use for lower_bound
// start: 73b6856ccfbfae1d5ccf6f8336388296  -
    struct comp {
        static bool by_slope;
        bool operator()(const pll& lhs, const pll& rhs) {
            if (by_slope) return lhs.first < rhs.first;
            return lhs.second < rhs.second;
        }
    };
// end
// start: 077c8a6fb239a3dd6a2a23f940bb59d1  -
    map<pll, pll, comp> hull;
    typedef map<pll, pll, comp>::iterator iterator;
    ll intercept(pll a, pll b) {
        if (a.first > b.first) swap(a, b);
        if (a.first == b.first) return LLONG_MIN;
        ll numer = a.second - b.second, denom = b.first - a.first;
        if (numer < 0) numer -= denom - 1;
        return (numer + denom - 1) / denom;
    }
// end
    // 'it' is the first iterator with slope greater than line.first
// start: 718596014c4d58e0ecfa1fc6794b5c75  -
    iterator fixbefore(iterator& it, pll& line) {
        if (it == hull.begin())
            return hull.insert(it, { {line.first, LLONG_MIN}, line});
        auto itb = prev(it);
        ll x = intercept(itb->second, line);
        if (x > itb->first.second) // found it
            return hull.insert(it, { {line.first, x}, line});
        hull.erase(itb);
        return fixbefore(it, line);
    }
// end
    // it is iterator of new element
// start: e975e0cf927072b70e08766263b2e2dc  -
    void fixafter(iterator it) {
        auto ita = next(it);
        if (ita == hull.end()) return;
        auto itaa = next(ita);
        pll line = ita->second;
        hull.erase(ita);
        ll x = intercept(it->second, line);
        if (itaa != hull.end() && x >= itaa->first.second) return fixafter(it);
        hull.insert( { {line.first, x}, line} );
    }
// end
// start: d0ebe465fc5c12206e64e79a4001966b  -
    void insert(ll slope, ll intercept) {
        comp::by_slope = true;
        pll l{slope, intercept};
        auto it = hull.lower_bound({slope, 0});
        // if already has something with the same slope
// 		start: f86f99248e3b05fcb6327a0fd311a5a1  -
        if (it != hull.end() && it->second.first == slope) {
            if (it->second.second >= intercept) return;
            hull.erase(it);
            return insert(slope, intercept);
        }
        // check if current envelope already covers all new line
        if (it != hull.end() && it != hull.begin()) {
            ll x = it->first.second;
            ll val = it->second.first*x + it->second.second;
            if (val >= slope * x + intercept) return;
        }
// 		end
        it = fixbefore(it, l);
        fixafter(it);
    }
// end
// start: 82ce2d68595654efa509573451f9e181  -
    ll maximum(ll x) {
        comp::by_slope = false;
        if (hull.empty()) return LLONG_MIN;
        pll upper = prev(hull.upper_bound({0, x}))->second;
        return upper.first * x + upper.second;
    }
// end
};
bool upper_envelope::comp::by_slope(true);
// end
