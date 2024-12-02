#include <iostream>
#include <vector>
#include <algorithm>

#define int long

struct Chamber {
  int i;
  int l;
  int g;
  int subtreeLen;
  int subtreeCnt;
};

std::ostream& operator << (std::ostream &o,const Chamber &c){
  return o<<"Chamber "<<c.i<<" has length "<<c.l<<" and reward "<<c.g<<", subtreeLen=" <<c.subtreeLen << ", subtreeCnt="<<c.subtreeCnt<<"\n";
}

struct Result {
  int T;
  int R;
  int numChambers;
};

std::ostream& operator << (std::ostream &o,const Result &r){
  return o<<"Result "<<"T="<<r.T<<", R="<<r.R<<", nC="<<r.numChambers<<"\n";
}


Result rec(std::vector<Chamber> &chambers, std::vector<std::vector<int>> &adj, int chamber) {
  int T = 0;
  int R = chambers[chamber].g;
  int numChambers = 1;
  
  for (int v : adj[chamber]) {
    Result subtree = rec(chambers, adj, v);
    
    R += subtree.R - subtree.numChambers*(T+chambers[v].l);
    T += subtree.T + 2*chambers[v].l;
    numChambers += subtree.numChambers;
    
    // std::cout << "considering " << chambers[v];
    // std::cout << "T=" << T << ", R=" << R<< ", nC=" << numChambers <<"\n";
  }
  return {T,R,numChambers};
}

signed main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n; std::cin >> n;
    
    std::vector<Chamber> chambers(n+1);
    chambers[0] = {0, 0, 0};
    for (int i=1; i<=n; i++) {
      int g; std::cin >> g;
      chambers[i] = {i, 0, g, 0, 1};
    }
    
    std::vector<std::vector<int>> adj(n+1, std::vector<int>());
    for (int i=0; i<n; i++) {
      int u, v, l; std::cin >> u >> v >> l;
      chambers[v].l = l;
      chambers[v].subtreeLen = l;
      adj[u].push_back(v);
    }
    
    auto comparator = [&](int a, int b) {
        return chambers[a].subtreeLen*chambers[b].subtreeCnt < chambers[b].subtreeLen*chambers[a].subtreeCnt;
    };
    
    for (int u=n; u>=0; u--) {
      for (int v : adj[u]) {
        chambers[u].subtreeLen += chambers[v].subtreeLen;// + chambers[v].l;
        chambers[u].subtreeCnt += chambers[v].subtreeCnt;
      }
      // std::cout << "chamber " << u << ": subtreeLen=" << chambers[u].subtreeLen << ", subtreeCnt=" << chambers[u].subtreeCnt << "\n";
      // prefer subtree with smaller ratio subtreeLen/subtreeCnt
      std::sort(adj[u].begin(), adj[u].end(), comparator);
    }
    
    std::cout << rec(chambers, adj, 0).R << "\n";
  }
}
