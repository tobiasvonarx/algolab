#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);

  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<vector<int>> M(n, vector<int>(n));
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        cin >> M[i][j];
      }
    }
    
    // S[i][j] is the parity of sub-matrix (0,i) (0,j)
    vector<vector<int>> S(n, vector<int>(n));
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        if (i==0 && j==0) {
          S[i][j] = M[i][j];
        } else if (i > 0 && j > 0) {
          S[i][j] = (S[i][j-1] + S[i-1][j] - S[i-1][j-1] + M[i][j] + 2) % 2;
        } else if (i > 0 && j == 0) {
          S[i][j] = (S[i-1][j] + M[i][j]) % 2;
        } else if (i == 0 && j > 0) {
          S[i][j] = (S[i][j-1] + M[i][j]) % 2;
        }
      }
    }
    
    int cnt = 0;
    for (int i_1 = 0; i_1<n; i_1++) {
      for (int i_2 = i_1; i_2<n; i_2++) {
        int evens = 0;
        int odds = 0;
        
        for (int j = 0; j<n; j++) {
          int parity;
          if (i_1 == 0) parity = S[i_2][j];
          else parity = (S[i_2][j] - S[i_1-1][j] + 2) % 2;
          
          if (parity == 0) evens++;
          else odds++;
        }
        cnt += evens * (evens - 1) / 2 + odds * (odds - 1) / 2 + evens;
      }
    }
    
    cout << cnt << "\n";
  }
}