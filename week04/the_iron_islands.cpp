#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, k, w; cin >> n >> k >> w;
    
    vector<int> c(n);
    for (int i=0; i<n; i++) cin >> c[i];

    vector<vector<int>> waterways(w);
    
    // costMap[cost to island] = island's waterway, dist from pyke
    unordered_map<int, vector<pair<int,int>>> costMap;
    vector<int> prefixSum(n); // cost from pyke to island
    vector<int> prefixLen(n); // dist from pyke to island
    vector<int> belonging(n); // which waterway does an island belong to
    
    for (int i=0; i<w; i++) {
      int l; cin >> l;
      waterways[i] = vector<int>(l);
      int psum = 0;
      for (int j=0; j<l; j++) {
        int island; cin >> island;
        
        waterways[i][j] = island;
        psum += c[island];
        
        if (costMap.find(psum) == costMap.end()) costMap[psum] = {make_pair(i, j)};
        else costMap[psum].push_back(make_pair(i,j));
      
        prefixSum[island] = psum;
        prefixLen[island] = j;
        belonging[island] = i;
      }
    }

    // sliding window and record length when val = k
    int mx_len = -1e9;
    
    // per waterway
    for (int i=0; i<w; i++) {
      int l = waterways[i].size();
      int left = 0; int right = 0;
      int val;
      
      // sliding window
      while (left < l && right < l) {
        if (left == 0) val = prefixSum[waterways[i][right]];
        else val = prefixSum[waterways[i][right]] - prefixSum[waterways[i][left-1]];
        
        if (val == k) {
          if (right-left+1 > mx_len) {
            mx_len = right-left+1;
          }
          left++;
          right++;
        } else if (val < k) { // expand window
          right++;
        } else { // smaller window
          if (left == right) {
            left++;
            right++;
          } else {
            left++;
          }
        }
      }
    }
    
    // paths that have two waterways (cross over pyke)
    for (int i=1; i<n; i++) {
      // island i to island j
      // where j is s.t. the total cost is k
      // and is on a different waterway
      int costLeft = k - prefixSum[i] + c[0];
      auto complement = costMap.find(costLeft);
      
      if (complement != costMap.end()) {
        for (auto otherIsland : complement->second) {
          int waterway = otherIsland.first;
          int len = otherIsland.second + prefixLen[i] + 1;
          if (otherIsland.second > 0 && belonging[i] != waterway && len > mx_len) {
            mx_len = len;
          }
        }
      }
    }
    
    if (mx_len < 0) cout << "0\n";
    else cout << mx_len << "\n";
  }
}