#include <iostream>
#include <vector>
#include <map>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int n, m; long D, T; std::cin >> n >> m >> D >> T;
    m++;
    
    std::vector<std::pair<long,long>> movements(n);
    std::vector<long> s(m);

    for (int i=0; i<n; i++) {
      long d, t; std::cin >> d >> t;
      movements[i] = std::make_pair(d, t);
    }
    
    s[0] = 0L; // 0 gulps
    for (int i=1; i<m; i++) {
      std::cin >> s[i];
    }
    
    int low = 0, high = m-1;
    int best = 1e9;
    while (low <= high) {
      int mid = low + (high - low)/2;
      long s_i = s[mid];
      // std::cout << "mid=" << mid << "\n";
      
      // split & list
      // check every subset of movements
      // condition: distance >= D in time < T
      int n1 = n / 2;
      int n2 = n - n1;

      std::map<long, long> lut;
      bool res = false;
      
      for (int x = 0; x < 1<<n1; x++) {
        long sumD = 0, sumT = 0;
        for (int bit=0; bit<n1; bit++) {
          if (x & 1<<bit) {
            sumD += movements[bit].first + s_i;
            sumT += movements[bit].second;
          }
          if (sumD >= D) break;
          if (sumT >= T) break;
        }
        lut[sumT] = std::max(sumD, lut[sumT]);
        // std::cout << "subset " << x << " has sumD=" << sumD << " and sumT=" << sumT << "\n";
        if (sumD >= D && sumT < T) {
          res = true;
          break;
        }
      }
      
      if (!res) {
        // look up subsets including elements in second list
        
        // first set that if sumT < sumT', then sumD' >= sumD
        // so we can lookup only one sumT (the complement)
        long currMax = 0;
        for (auto it=lut.begin(); it != lut.end(); it++) {
          if (it->second > currMax) {
            currMax = it->second;
          } else if (it->second < currMax) {
            it->second = currMax;
          }
        }

        for (int x = 0; x < 1<<n2; x++) {
          long sumD = 0, sumT = 0;
          for (int bit=0; bit<n2; bit++) {
            if (x & 1<<bit) {
              sumD += movements[n1+bit].first + s_i;
              sumT += movements[n1+bit].second;
            }
            if (sumD >= D) break;
            if (sumT >= T) break;
          }
          // lookup complement (max such that we stay < T)
          if (sumT < T) {
            auto it = lut.upper_bound(T-sumT-1); 
            
            if (it != lut.begin()) {
              it--;
              sumD += it->second;
            }
          }
          // std::cout << "subset " << x << " has sumD=" << sumD << " and sumT=" << sumT << "\n";
          if (sumD >= D && sumT < T) {
            res = true;
            break;
          }
        }
        
      }
      
      if (!res) { // increase
        low = mid + 1;
      } else { // decrease
        high = mid - 1;
        best = std::min(best, mid);
      }
    }
    if (best == 1e9) std::cout << "Panoramix captured\n";
    else std::cout << best << "\n";
  }
}