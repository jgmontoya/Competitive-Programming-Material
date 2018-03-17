// tags: DP, HIV (heaviest increasing subsequence), RMQ, fenwick tree,
// domain compression
#include <bits/stdc++.h> // add almost everything in one shot
using namespace std;
// defines
#define rep(i,a,b) for(int i = a; i <= b; ++i)
#define invrep(i,b,a) for(int i = b; i >= a; --i)
#define umap unordered_map
#define uset unordered_set
// typedefs
typedef unsigned int uint;
typedef long long int ll;
typedef vector<int> vi;
typedef pair<int,int> ii;
typedef tuple<int,int,int> iii;
// -------------------------------

struct Triplet {
    int b; int f; ll d;
    bool operator<(const Triplet& o) const {
        if (b < o.b) return true;
        if (b == o.b and f > o.f) return true;
        return false;
    }
};

struct FenwickTree {
    vector<ll> ft;
    FenwickTree(int n) {
        ft.assign(n+1,0);
    }
    ll rmq(int b) {
        ll ans = 0;
        for (; b; b -= (b & -b)) ans = max(ans, ft[b]);
        return ans;
    }
    void update(int b, ll new_val) {
        for(; b < ft.size(); b += (b & -b)) ft[b] = max(ft[b], new_val);
    }
};

int main() {
    int N;
    scanf("%d", &N);
    map<ii,ll> bf2d; // map (b,f) -> d, notice this is an ordered map
    set<int> fset; // set of unique f values
    while (N--) {
        int b,f,d;
        scanf("%d%d%d",&b,&f,&d);
        fset.insert(f); // add f to fset
        // merge all (b,f) pairs with the same b and f
        // into single points
        ii q(b,f);
        auto it = bf2d.find(q);
        if (it == bf2d.end()) {
            bf2d[q] = d;
        } else {
            it->second += d;
        }
    }

    // reassign f values in a compact domain
    // making sure to preserve order
    umap<int,int> compact_f_map;
    int maxf = 0;
    for (auto& f : fset) {
        compact_f_map[f] = ++maxf;
    }

    // collect all b,f,d in triplets and sort them
    // lexicographically: increasing in b, decreasing in f
    vector<Triplet> triplet_list(bf2d.size());
    int i = 0;
    for (auto& it : bf2d) {
        Triplet& t = triplet_list[i++];
        t.b = it.first.first;
        t.f = compact_f_map[it.first.second];
        t.d = it.second;
    }
    sort(triplet_list.begin(), triplet_list.end());

    // FT to keep track of maximum subsequence weight
    // seen so far such that it ends in value f,
    // for 1 <= f <= maxf
    FenwickTree ft(maxf);

    // Find HIS for each point, from left to right,
    // reusing stored HIS of previous points by quering
    // the fenwick tree. Then update the fenwick tree
    // with this new value.
    for (Triplet& t : triplet_list) {
        ft.update(t.f, ft.rmq(t.f-1) + t.d);
    }

    // print global maximum
    printf("%lld\n", ft.rmq(maxf));
    return 0;
}