#include <iostream>
#include <vector>
#include <functional>
#include <limits>

struct Result {
  int mustTake;
  int tookParent;
  int notTookParent;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n; std::cin >> n;
    
    std::vector<std::vector<int>> adj(n, std::vector<int>());
    for (int i=0; i<n-1; i++) { // edges of tree
      int u, v; std::cin >> u >> v;
      adj[u].push_back(v);
    }
    
    std::vector<int> c(n);
    for (int i=0; i<n; i++) std::cin >> c[i];
    
    std::function<Result(int)> dfs = [&](int u) {
      Result result;

      std::vector<Result> children;
      children.reserve(adj[u].size());
      // process children    
      for (int v : adj[u]) {
        children.push_back(dfs(v));
      }
      
      // min cost if must take u
      result.mustTake = c[u];
      for (Result child : children) result.mustTake += child.tookParent;
      
      // min cost if took parent of u
      // either still take it, or don't
      int dontTake = 0;
      for (Result child : children) dontTake += child.notTookParent;
      result.tookParent = std::min(result.mustTake, dontTake);
      
      // min cost if didnt take parent of u
      // either take u, or take at least one child of u (s.t. u will be covered)
      
      // take at least one child => which child?
      // min v in adj[u] [ sum_(v' != v) v'.notTookParent + v.mustTake ]
      // i.e. min v in adj[u] [ sum_v' v'.notTookParent + (v.mustTake - v.notTookParent) ]
      // i.e. dontTake + min v in adj[u] [ (v.mustTake - v.notTookParent) ]
      int minLoss = std::numeric_limits<int>::max();
      for (Result child : children) minLoss = std::min(minLoss, child.mustTake - child.notTookParent);
      result.notTookParent = std::min(result.mustTake, dontTake + minLoss);
      
      return result;
    };
    
    Result root = dfs(0);
    std::cout << root.notTookParent << "\n";
  }
}
