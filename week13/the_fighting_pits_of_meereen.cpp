#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

#undef NDEBUG
#include <cassert>

typedef std::unordered_map<std::string,int> M;
typedef std::vector<M> VM;
typedef std::vector<VM> VVM;

int calcReward(std::string prevState, int t, std::vector<int> &x, int m, bool north) {
  int offset = north?0:2;
  std::set<char> unique = {'.'};
  if (m==3) {
    unique.insert(prevState[offset]);
  }
  unique.insert(prevState[offset+1]);
  unique.insert(x[t]);
  // std::cout << "f(" << (int)prevState[offset+1] << " " << x[t];
  // std::cout << ",north=" << north << ") = " << 1000*(unique.size()-1) << "\n";
  return 1000*(unique.size()-1);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, k, m; std::cin >> n >> k >> m;
    std::vector<int> x(n+1);
    x[0] = -1;
    for (int i=0; i<n; i++) std::cin >> x[i+1];
    
    // all valid states must have -11<=p-q<=11 as else, excitement<0
    // equivalently, 0<=p-q+11<=22
    // dp[t][p-q+11] = all possible states after t rounds, with p=p, q=q
    // where a state is {prev_north, curr_north, prev_south, curr_south}
    
    VVM dp(n+1, VM(23, M()));
    
    std::string empty = "....";
    
    dp[0][11][empty] = 0;
    int best = -1;
    
    for (int t=1; t<=n; t++) {
      best = -1;
      for (int diff=0; diff<23; diff++) {
        // t = p+q
        // diff = p-q+11
        if ((t+diff-11)%2!=0) continue;
        
        int p = (t+diff-11)/2;
        int q = t-p;
        assert(p+q == t);
        
        if (p<0 || q<0) continue;
        
        // std::cout << "p-q=" << diff-11 << "\n";
        
        if (p > 0 && diff > 0) { // consider north
          for (auto it : dp[t-1][diff-1]) {
            std::string state = it.first;
            int reward = it.second;
            
            reward += calcReward(state, t, x, m, true) - (1<<std::abs(diff-11));
            if (reward < 0) continue;
            
            std::string newState = state;
            newState[0] = state[1];
            newState[1] = x[t];
            
            dp[t][diff][newState] = std::max(dp[t][diff][newState], reward);
            best = std::max(reward, best);
          }
        }
        if (q > 0 && (diff+1)<23) { // consider south
          for (auto it : dp[t-1][diff+1]) {
            std::string state = it.first;
            int reward = it.second;
            
            reward += calcReward(state, t, x, m, false) - (1<<std::abs(diff-11));
            if (reward < 0) continue;
            
            std::string newState = state;
            newState[2] = state[3];
            newState[3] = x[t];
            
            dp[t][diff][newState] = std::max(dp[t][diff][newState], reward);
            best = std::max(reward, best);
          }
        }
        // std::cout << "t=" << t << ", best=" << best << "\n";
      }
    }
    
    std::cout << best << "\n";
  }
}
