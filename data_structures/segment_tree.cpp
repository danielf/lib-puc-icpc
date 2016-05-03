// start: 0e35a8712a3740199e0ef1531cc3bc10  - whole file
template<typename T> struct seg_tree {
// start: e9dbfee7e8dd936834dbe5db53542d6e  -
    int S;

    T zero;
    vector<T> value;

    seg_tree<T>(int _S, T _zero = T()) {
        S = _S, zero = _zero;
        value.resize(2*S+1, zero);
    }
// end

// start: a3dd1de6a4cbb100022952f330feeea4  -
    void set_leaves(vector<T> &leaves) {
        copy(leaves.begin(), leaves.end(), value.begin() + S);

        for (int i = S - 1; i > 0; i--) 
            value[i] = value[2 * i] + value[2 * i + 1];
    }
// end

// start: 47cc47e3ee7a6b5e7a63a98e891ba2b9  -
    void upd(int i, T v) {
        i += S;
        value[i] = v;
        while(i>1){
            i/=2;
            value[i] = value[2*i] + value[2*i+1];
        }
    }
// end

// start: d4b1c3080ac393817bc6a82e0bf80367  -
    T query(int i, int j) {
        T res_left = zero, res_right = zero;
        for(i += S, j += S; i <= j; i /= 2, j /= 2){
            if((i&1) == 1) res_left = res_left + value[i++];
            if((j&1) == 0) res_right = value[j--] + res_right;
        }
        return res_left + res_right;
    }
// end
};
// end
