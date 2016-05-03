struct upper_envelope {
    typedef pair<ll, ll> pll;
    // key of map is a pll. first is slope, second is "break point"
    // it's increasing in both, by_slope controls which to use for lower_bound
    struct comp {
        static bool by_slope;
        bool operator()(const pll& lhs, const pll& rhs) {
            if (by_slope) return lhs.first < rhs.first;
            return lhs.second < rhs.second;
        }
    };
    map<pll, pll, comp> hull;
    typedef map<pll, pll, comp>::iterator iterator;
    ll intercept(pll a, pll b) {
        if (a.first > b.first) swap(a, b);
        if (a.first == b.first) return LLONG_MIN;
        ll numer = a.second - b.second, denom = b.first - a.first;
        if (numer < 0) numer -= denom - 1;
        return (numer + denom - 1) / denom;
    }
    // 'it' is the first iterator with slope greater than line.first
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
    // it is iterator of new element
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
    void insert(ll slope, ll intercept) {
        comp::by_slope = true;
        pll l{slope, intercept};
        auto it = hull.lower_bound({slope, 0});
        // if already has something with the same slope
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
        it = fixbefore(it, l);
        fixafter(it);
    }
    ll maximum(ll x) {
        comp::by_slope = false;
        if (hull.empty()) return LLONG_MIN;
        pll upper = prev(hull.upper_bound({0, x}))->second;
        return upper.first * x + upper.second;
    }
};
bool upper_envelope::comp::by_slope(true);

