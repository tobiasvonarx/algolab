#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n; cin >> n;

    int evens = 0;
    int odds = 0;
    
    int prefixSum = 0;
    
    for (int i=0; i<n; i++) {
      int x; cin >> x;
      prefixSum += x;
      
      if (prefixSum % 2 == 0) evens++;
      else odds++;
    }
    
    int cnt = evens * (evens - 1) / 2 + odds * (odds - 1) / 2 + evens;
  
    cout << cnt << "\n";
  }
}