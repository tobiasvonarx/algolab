#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

#undef NDEBUG
#include <cassert>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int n; std::cin >> n;
    
    std::vector<std::pair<int,int>> v(n);
    for (int i=0; i<n; i++) {
      int l, p; std::cin >> l >> p;
      v[i] = std::make_pair(p, l);
    }
    
    std::sort(v.begin(), v.end());
    
    int boatCnt = 1; // place boat 0 to end at p_0
    int prevBoatEnd = v[0].first;
    int prevBoatStart = v[0].first - v[0].second;
    //std::cout << prevBoatStart << "->" << prevBoatEnd << "\n";
    
    for (int i=1; i<n; i++) {
      // want to place boat i
      int p=v[i].first, l=v[i].second;

      if (prevBoatEnd <= p) { // no conflict
        // place as far left as possible
        boatCnt++;
        prevBoatStart = std::max(prevBoatEnd + 1, p-l+1);
        prevBoatEnd = prevBoatStart + l - 1;
      } else { // there is 1 boat covering the entire ring, switch if taking this boat ends before that
        int candBoatStart = std::max(prevBoatStart, p-l+1);
        int candBoatEnd = candBoatStart + l - 1;
        //std::cout << "cand: " << candBoatStart << "->" << candBoatEnd << "\n";
        if (candBoatEnd < prevBoatEnd) {
          prevBoatStart = candBoatStart;
          prevBoatEnd = candBoatEnd;
        }
      }
      //std::cout << prevBoatStart << "->" << prevBoatEnd << "\n";
    }
    
    std::cout << boatCnt << "\n";
  }
}