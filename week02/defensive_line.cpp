#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m, k; cin >> n >> m >> k;
    vector<int> v(n);
    for (int i=0; i<n; i++) cin >> v[i];
    
    int left = 0; int right = 0;
    int sum = v[0]; // current window
    
    map<int, int> intervals; // map end -> start
    
    while (left < n && right < n) {
      if (sum < k) {
        right++;
        sum += v[right];
      } else if (sum == k) {
        // cout << "found window of size " << right-left+1 << "\n";
        intervals[right+1] = left;
        sum -= v[left];
        left++;
        right++;
        sum += v[right];
      } else {
        sum -= v[left];
        if (left == right) {
          right++;
          sum = v[right];
        }
        left++;
      }
    }
    
    // dp[i][j] best value ending before defender i using j attackers
    vector<vector<int>> dp(n+1, vector<int>(m+1));
    for (int i=0; i<=n; i++) dp[i][0] = 0;
    for (int j=0; j<=m; j++) dp[0][j] = 0;
    
    for (int i=1; i<=n; i++) { // end before defender i
      for (int j=1; j<=m; j++) { // #attackers
        if (intervals.find(i) == intervals.end()) {
          dp[i][j] = dp[i-1][j];
        } else {
          int start = intervals[i];
          int length_interval = i - start;
          //cout << "[" << start << "," << i << "), L=" << length_interval << "\n";
          dp[i][j] = max(dp[start][j-1] + length_interval, dp[i-1][j]);
        }
      }
      // cout << "best using " << j << " attackers is " << dp[n][j] << "\n";
    }
    
    if (dp[n][m-1] == dp[n][m]) cout << "fail\n";
    else cout << dp[n][m] << "\n";
  }
}