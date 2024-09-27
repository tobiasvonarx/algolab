#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    int r, b; cin >> r >> b;
    vector<vector<int>> edges(n); // multimap approach slow
    for (int i=0; i<m; i++) {
      int u, v; cin >> u >> v;
      edges[u].push_back(v);
    }
    
    // black and red meeple are independent
    // Sherlock minimizes red moves and Moriarty minimizes black moves while
    // Sherlock maximizes black moves and Moriarty maximizes red moves
    // {min,max}_dp[i] is moves to get from i to n while curr. minimizing/maximizing
    vector<int> min_dp(n+1,numeric_limits<int>::max());
    vector<int> max_dp(n+1,numeric_limits<int>::min());
    min_dp[n] = 0; max_dp[n] = 0;
    
    for (int u=n-1; u>0; u--) {
      for (auto const &v : edges[u]) {
        min_dp[u] = min(min_dp[u],max_dp[v]+1);
        max_dp[u] = max(max_dp[u],min_dp[v]+1);
      }
    }
    
    if (min_dp[r] < min_dp[b]) { // red arrives before black
      cout << "0\n";
    } else if (min_dp[r] > min_dp[b]) { // black arrives before red
      cout << "1\n";
    } else {
      if (min_dp[r] % 2 == 0) {
        // last turn Sherlock moves black and loses
        cout << "1\n";
      } else {
        // last turn Sherlock moves red and wins
        cout << "0\n";
      }
    }
  }
}