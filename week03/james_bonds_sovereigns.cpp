#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int mx = 2e3;
vector<vector<int>> memo(mx, vector<int>(mx));

int rec(vector<int> &x, int l, int r, int n, int m, int k) {
  bool k_turn = ((n-1-r + l)%m) == k;

  if (l == r) {
    if (k_turn) return x[l];
    else return 0;
  }
  
  if (memo[l][r] != -1) return memo[l][r];

  if (k_turn) {
    memo[l][r] = max(rec(x, l+1, r, n, m, k) + x[l], rec(x, l, r-1, n, m, k) + x[r]);
  } else {
    memo[l][r] = min(rec(x, l+1, r, n, m, k), rec(x, l, r-1, n, m, k));  
  }
  
  return memo[l][r];
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m, k; cin >> n >> m >> k;
    vector<int> x(n);
    for (int i=0; i<n; i++) cin >> x[i];
    
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        memo[i][j] = -1;
      }
    }
    
    cout << rec(x, 0, n-1, n, m, k) << "\n";
  }
}