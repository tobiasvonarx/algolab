#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int n, m; std::cin >> n >> m;
    
    std::vector<int> s(n);
    std::multiset<int, std::greater<int>> w;
    
    for (int i=0; i<n; i++) std::cin >> s[i];
    for (int i=0; i<m; i++) {
      int w_i; std::cin >> w_i;
      w.insert(w_i);
    }
    
    std::sort(s.begin(), s.end(), std::greater<int>());
    
    if (s[0] < *w.begin()) {
      std::cout << "impossible\n";
      continue;
    }
    
    // have a set storing boxes to be transported
    // each round: each friend carries the strongest box it can carry among those left
    int rounds = 0;
    while (w.size() > 0) {
      int friendIdx = 0;
      for (auto it=w.begin(); it!=w.end(); it=w.lower_bound(s[friendIdx])) {
        // found lower bound box, i.e. s[friendIdx] >= *it
        // => friend carries it
        w.erase(it);
        friendIdx++;
        
        // all friends already carry something this round
        if (friendIdx == n) break;
      }
      rounds++;
    }
    
    std::cout << rounds*3 - 1 << "\n";
  }
}