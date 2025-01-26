///4
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

typedef std::priority_queue<std::pair<int,int>> max_heap;
typedef std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> min_heap;

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m, k; std::cin >> n >> m >> k;
    
    std::vector<int> h(n);
    for (int i=0; i<n; i++) {
      std::cin >> h[i];
    }
    
    std::vector<int> prev(n);
    prev[0] = -1;
    std::vector<bool> leaf(n, true);
    for (int i=0; i<n-1; i++) {
      int u, v; std::cin >> u >> v;
      prev[v] = u;
      leaf[u] = false;
    }
    
    std::vector<bool> solution(n, false);
    std::vector<int> mapping(n);
    std::vector<bool> vis(n, false);

    bool existsSol = false;
    
    for (int i=0; i<n; i++) {
      if (leaf[i]) {
        int u = i;
        max_heap maxheap;
        min_heap minheap;
        for (int j=0; u != -1; j++, u = prev[u]) {
          mapping[j] = u;
          maxheap.push({h[u],j});
          minheap.push({h[u],j});
          
          // pop relevant max/mins out of the sequence
          while (maxheap.top().second + m <= j) maxheap.pop();
          while (minheap.top().second + m <= j) minheap.pop();
          
          // sequence is a potential candidate
          if (j+1 >= m) {
            int contrast = maxheap.top().first - minheap.top().first;
            if (contrast <= k) existsSol = solution[u] = true;
            if (vis[mapping[j+1-m]]) break;
            vis[mapping[j+1-m]] = true;
          }
        }
      }
    }
    
    if (existsSol) {
      for (int i=0; i<n; i++) {
        if (solution[i]) std::cout << i << " ";
      }
    } else {
      std::cout << "Abort mission";
    }
    std::cout << "\n";
  }
}
