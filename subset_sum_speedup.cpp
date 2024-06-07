#include<bits/stdc++.h>
using ll = long long;
using ull = unsigned long long;
using namespace std;

// https://codeforces.com/blog/entry/98663

const int D_MAX = 20000;

int t;
int n, c;
vector<int> a;

// cache[0] = dp(total, r - 1)
// cache[1] = dp(total, r)
int cache[2][2 * D_MAX + 1];
inline void set_dp(int total, bool cur_r, int new_l){
    cache[cur_r][total - (c - D_MAX)] = new_l;
}
inline int get_dp(int total, bool cur_r){
    return cache[cur_r][total - (c - D_MAX)];
}

int subset_sum(){
    int d = 0;
    for(int i = 0; i < n; ++i) d = max(d, a[i]);
    int k, k_sum = 0;
    for(k = 0; k < n; ++k){
        if(k_sum + a[k] > c) break;
        k_sum += a[k];
    }
    if(k == n) {
        if(k_sum <= c) return k_sum;
        return 0;
    }

    memset(cache, -1, sizeof(cache));
    set_dp(k_sum, false, k);
    for(int i = k; i < n; ++i){
        // For total is in [C-D, C), extend r, transitioning to
        // dp(total + a[i], r) = dp(total, r - 1) or dp(total, r) = dp(total, r - 1)
        memcpy(cache[1], cache[0], sizeof(cache[0]));
        for(int total = c - 1; total >= c - d; --total){
            if(get_dp(total + a[i], true) < get_dp(total, false))
                set_dp(total + a[i], true, get_dp(total, false));
        }
        // For total is in [C, C+D], extend l, transitioning to 
        // dp(total - a[l'], r) = l' where dp(total, r - 1) <= l' < dp(total, r)
        for(int total = c + d; total >= c; --total){
            for(int prev_l = get_dp(total, true) - 1; prev_l >= max(get_dp(total, false), 0); --prev_l){
                if(get_dp(total - a[prev_l], true) < prev_l)
                    set_dp(total - a[prev_l], true, prev_l);
            }
        }
        memcpy(cache[0], cache[1], sizeof(cache[0]));
    }

    for(int i = c; i >= max(c - d, 0); --i){
        if(get_dp(i, false) != -1) 
            return i;
    }
    return 0;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    cin >> t;
    while(t--){
        cin >> n >> c;
        a.clear();
        a.resize(n);
        for(int i = 0; i < n; ++i) cin >> a[i];
        cout << subset_sum() << '\n';
    }
}