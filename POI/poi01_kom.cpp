#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <functional>
#include <vector>
#include <numeric>
#include <deque>
#include <utility>
#include <bitset>
using namespace std;
typedef long long lint;
typedef long double llf;
typedef pair<int, int> pi;


const int MAXN = 150000;
struct strongly_connected{
    vector<int> gph[MAXN];
    vector<int> rev[MAXN];
    vector<int> dfn;
    int comp[MAXN], piv;
    bool vis[MAXN];

    void clear(){
        for(int i=0; i<MAXN; i++){
            gph[i].clear();
            rev[i].clear();
        }
    }

    void add_edge(int s, int e){
        gph[s].push_back(e);
        rev[e].push_back(s);
    }

    void dfs(int x){
        vis[x] = 1;
        for(auto &i : gph[x]){
            if(!vis[i]) dfs(i);
        }
        dfn.push_back(x);
    }

    void rdfs(int x, int p){
        comp[x] = p;
        for(auto &i : rev[x]){
            if(!comp[i]) rdfs(i, p);
        }
    }

    void get_scc(int n){
        dfn.clear();
        memset(comp, 0, sizeof(comp));
        memset(vis, 0, sizeof(vis));
        piv = 0;
        for(int i=0; i<n; i++){
            if(!vis[i]) dfs(i);
        }
        reverse(dfn.begin(), dfn.end());
        for(auto &i : dfn){
            if(!comp[i]) rdfs(i, ++piv);
        }
    }
}scc;

struct twosat{
    strongly_connected scc;
    int n;
    void init(int _n){ scc.clear(); n = _n; }
    int NOT(int x){ return x >= n ? (x - n) : (x + n); }
    void add_edge(int x, int y){
        if((x >> 31) & 1) x = (~x) + n;
        if((y >> 31) & 1) y = (~y) + n;
        scc.add_edge(x, y), scc.add_edge(NOT(y), NOT(x));
    }
    bool satisfy(int *res){
        scc.get_scc(2*n);
        for(int i=0; i<n; i++){
            if(scc.comp[i] == scc.comp[NOT(i)]) return 0;
            if(scc.comp[i] < scc.comp[NOT(i)]) res[i] = 0;
            else res[i] = 1;
        }
        return 1;
    }
}twosat;

int res[MAXN];

int main(){
    int n, m;
    scanf("%d %d",&n,&m);
    twosat.init(n);
    while(m--){
        int l, r;
        scanf("%d %d",&l,&r);
        int lv = (l - 1) / 2, rv = (r - 1) / 2;
        if(l % 2 == 0) lv = twosat.NOT(lv);
        if(r % 2 == 0) rv = twosat.NOT(rv);
        twosat.add_edge(twosat.NOT(lv), rv);
    }
    if(!twosat.satisfy(res)) puts("NIE");
    else{
        for(int i=0; i<n; i++){
            printf("%d\n", res[i] + 2 * i + 1);
        }
    }
}

