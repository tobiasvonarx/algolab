#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
typedef vector<vector<vector<int>>> VVVI;
VVVI memo(2500, vector<vector<int>>(2500, vector<int>(2)));

// rec(v, i, j, turn) is
// winnings guaranteed when it is turn's turn to choose
// among coins i to j left
int rec(vector<int> &v, int i, int j, int turn) {
  if (i == j) { // one coin left
    if (turn) return v[i];
    else return 0;
  }
  
  if (memo[i][j][turn] != -1) return memo[i][j][turn];
  int pickLeft = (turn?v[i]:0) + rec(v, i+1, j, 1-turn);
  int pickRight = (turn?v[j]:0) + rec(v, i, j-1, 1-turn);
  
  // cout << "turn "<< turn << " i=" << i << ", j=" << j << ": ";
  // cout << pickLeft << " / " << pickRight << "\n";
  
  if (turn) memo[i][j][turn] = max(pickLeft, pickRight);
  else memo[i][j][turn] = min(pickLeft, pickRight);
  
  return memo[i][j][turn];
}
int main() {
  ios_base::sync_with_stdio(false);

  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<int> v(n);
    for (int i=0; i<n; i++) cin >> v[i];
    
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        memo[i][j][0] = -1;
        memo[i][j][1] = -1;
      }
    }
    cout << rec(v, 0, n-1, 1) << "\n";
  }
}