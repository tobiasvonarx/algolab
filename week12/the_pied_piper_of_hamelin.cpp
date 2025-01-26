#include <iostream>
#include <vector>
#include <algorithm>


void solve() {
  int n, m; std::cin >> n >> m;
  std::vector<std::vector<int>> graph(n, std::vector<int>(n, -1));
  std::vector<std::vector<long>> dp(n, std::vector<long>(n, -1L));
  dp[0][0] = 0L;

  for (int i = 0; i < m; i++) {
    int u, v, r; std::cin >> u >> v >> r;
    graph[u][v] = r;
  }
  
  for (int i = 1; i < n; i++) {
    for (int u = 0; u < i || (i == n-1 && u < n); u++) {
      for (int v = 0; v < i || (i == n-1 && v < n); v++) {
        if (dp[u][v] < 0L) continue;
        
        // 0->u->i and j->v->0
        if (graph[u][i] > 0) {
          dp[i][v] = std::max(dp[i][v], dp[u][v] + graph[u][i]);
        }
        
        // 0->u->j and i->v->0
        if (graph[i][v] > 0) {
          dp[u][i] = std::max(dp[u][i], dp[u][v] + graph[i][v]);
        }
      }
    }
  }
  
  std::cout << dp[n-1][n-1] << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  while (t--) solve();
}
