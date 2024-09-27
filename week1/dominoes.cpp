#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    vector<int> hs(n);
    for (int i=0; i<n; i++) cin >> hs[i];

    int hLeft = 1;
    int i = 0;
    while (hLeft && i < n) {
      hLeft--;
      hLeft = max(hLeft, hs[i]-1);
      i++;
    }
    cout << i << "\n";
  }
}