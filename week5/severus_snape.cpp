#include <iostream>
#include <vector>
#include <algorithm>

#define INVALID -2L
#define UNKNOWN -1L

typedef std::vector<std::vector<std::vector<long>>> VVVI;
typedef std::vector<std::vector<long>> VVI;

VVVI memo(101, VVI(101, std::vector<long>(1025)));

// max power while using x of the first X potionsA and happiness >= h
long rec(const std::vector<std::pair<long,int>> &potionsA, int x, int X, int h) {
  // std::cout << "rec(" << x << ", " << X << ", " << h << ")" << std::endl;
  if (x > X) return INVALID;
  if (x == 0 && h <= 0) return 0L;
  if (x == 0 && h > 0) return INVALID;

  if (memo[x][X][h] != UNKNOWN) return memo[x][X][h];

  long dontTakeCurr = rec(potionsA, x, X-1, h);
  
  long currP = potionsA[X-1].first;
  int currH = potionsA[X-1].second;
  
  long takeCurr = rec(potionsA, x-1, X-1, std::max(0,h-currH));
  
  if (dontTakeCurr != INVALID && takeCurr != INVALID) {
    memo[x][X][h] = std::max(dontTakeCurr, takeCurr + currP);
  } else if (dontTakeCurr == INVALID && takeCurr == INVALID) {
    memo[x][X][h] = INVALID;
  } else if (dontTakeCurr == INVALID) {
    memo[x][X][h] = takeCurr + currP;
  } else if (takeCurr == INVALID) {
    memo[x][X][h] = dontTakeCurr;
  }
  
  return memo[x][X][h];
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int n, m; std::cin >> n >> m;
    long a, b; std::cin >> a >> b;
    long P, W; int H; std::cin >> P >> H >> W;
    
    std::vector<std::pair<long,int>> potionsA(n); // P^ H^ Wv
    for (int i=0; i<n; i++) {
      long p_i; int h_i; std::cin >> p_i >> h_i;
      potionsA[i] = std::make_pair(p_i, h_i);
    }
    
    std::vector<long> potionsB(m); // Pv H= W^
    for (int i=0; i<m; i++) std::cin >> potionsB[i];
    
    std::sort(potionsB.begin(), potionsB.end(), std::greater<long>());
    
    for (int x=0; x<=100; x++) {
      for (int X=0; X<=100; X++) {
        for (int h=0; h<=1024; h++) {
          memo[x][X][h] = UNKNOWN;
        }
      }
    }

    // std::cout << "P=" << P << ", H=" << H << ", W=" << W << "\n";
    
    int best = -1;
    for (int cntA=1; cntA<=n; cntA++) {
      long p = rec(potionsA, cntA, n, H);
      if (p < P) continue;
      long w = - a * (long)cntA;
      
      // as many typeB potions as needed to get w >= W
      int cntB = 0;
      bool fail = false;
      while (w < W) {
        if (cntB == m) {
          fail = true;
          break;
        }
        w += potionsB[cntB];
        cntB++;
      }
      if (fail) break;
      
      // power lost due to these typeB potions
      p -= b * (long)cntB;
      
      // std::cout << "cntA: " << cntA << ", cntB: " << cntB << ", p:" << p << ", h:" << H << ", w:" << w << "\n";
      if (p >= P) {
        best = cntA+cntB;
        break;
      }
    }
    
    std::cout << best << "\n";
  }
}