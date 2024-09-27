#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, k; cin >> n >> k;

    vector<int> v(n+1,0);
    for (int i=0; i<n; i++) cin >> v[i];
    
    int i = 0; int j = 0;
    int sum = v[0]; // from i to j
    
    int best_i = 0; int best_j = 0;
    int best_val = abs(sum - k);
    
    while (i < n && j < n) { // not overshoot (v has padding of 1 elem)
      if (sum < k) { // too small, increase range on right
        j++;
        sum += v[j];
      } else if (sum > k) { // too big, decrease range on left
        if (i == j) {
          j++;
          sum = v[j];
        } else {
          sum -= v[i];
        }
        i++;
      } else break; // found optimal
       
      if (abs(sum - k) < best_val) {
        best_i = i;
        best_j = j;
        best_val = abs(sum - k);
      }
    }
    
    cout << best_i << " " << best_j << "\n";
  }
}