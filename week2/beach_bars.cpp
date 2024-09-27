#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    
    multiset<int> x;
    for (int i=0; i<n; i++) {
      int x_i; cin >> x_i;
      x.insert(x_i);
    }
    
    int mn = *x.begin();
    int mx = *x.rbegin();
    
    auto left = x.begin();
    auto right = x.begin();
    
    int in_range = 1;
    
    int best_cnt = 0;
    set<int> best_cnts;
    
    int mn_dist = 100;
    //set<int> mn_dists;

    for (int i=mn; i<=mx; i++) {
      // we are on coord i, update left and right #parasols
      
      while (left != x.end() && *left < i - 100) { // point to first parasol in interval
        left++;
        in_range--;
      }
      
      while (next(right,1) != x.end() && *(next(right,1)) <= i + 100) { // point to last parasol in interval
        right++;
        in_range++;
      }

      if (in_range >= best_cnt) {
        if (in_range > best_cnt) {
          best_cnt = in_range;
          best_cnts.clear();
          mn_dist = 100;
        }
        int dist = max(*right-i, i-*left);
        if (dist <= mn_dist) {
          if (dist < mn_dist) {
            mn_dist = dist;
            best_cnts.clear();
          }
          best_cnts.insert(i);
        }
      }
      
      // cout << "i=" << i << ": " << in_range << " " << mn_dist << "\n";
    }

    cout << best_cnt << " " << mn_dist << "\n";
    for (auto const &coord : best_cnts) cout << coord << " ";
    cout << "\n";
  }
}