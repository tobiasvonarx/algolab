#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

int overlapSize(const std::vector<int> &vec1, const std::vector<int> &vec2, int k) {
    int overlap = 0;
    for (int len = 1; len <= k; len++) {
        bool ok = true;
        for (int i = 0; i < len; i++) {
            if (vec1[k - len + i] != vec2[i]) {
                ok = false;
                break;
            }
        }
        if (ok) overlap = len;
    }
    return overlap;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m; std::cin >> n >> m;
    int k, d; std::cin >> k >> d;
    
    VVI plans(m, VI(k));
    VVI index(n);

    for (int i=0; i<m; i++) {
      for (int j=0; j<k; j++) {
        std::cin >> plans[i][j];
      }
      index[plans[i][k-1]].push_back(i);
    }
    
    // dp[p] = fastest time to use plan p
    std::vector<int> dp(m,-1);
    for (int i : index[0]) {
      dp[i] = k;
    }
    
    for (int h=1; h<n; h++) {
      // previously used some plan for h-1
      for (int curr : index[h]) {
        int best = 1e9;
        for (int prev : index[h-1]) {
          if (dp[prev] == -1) continue;
          int overlap = overlapSize(plans[prev], plans[curr], k);
          // std::cout << prev << "->" << curr << ": overlap of " << overlap << "\n";
          
          // plans[curr][overlap:] must be non-eradicated
          bool ok = true;
          for (int j=overlap; j<k; j++) {
            if (plans[curr][j] < h) { // cant cut eradicated head
              ok = false;
              break;
            }
          }
          if (!ok) continue;
          
          best = std::min(best, dp[prev]+k-overlap);
        }
        if (best != 1e9) dp[curr] = best;
        // std::cout << "dp[" << curr << "]=" <<  dp[curr] << "\n";
      }
    }
    
    int T = 1e9;
    for (int lastHeadPlan : index[n-1]) {
      if (dp[lastHeadPlan] != -1) {
        T = std::min(dp[lastHeadPlan], T);
      }
    }
    
    if (T != 1e9) std::cout << T << "\n";
    else std::cout << "Impossible!\n";
  }
}
