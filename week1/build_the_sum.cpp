#include <iostream>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;
    int sum = 0;
    for (int i=0; i<n; i++) {
      int a_i; cin >> a_i;
      sum += a_i;
    }
    cout << sum << "\n";
  }
}