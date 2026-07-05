 #include <iostream>
 #include <vector>
 #include <stack>
 #include <algorithm>
 
 int main() {
   std::ios_base::sync_with_stdio(false);
   
   int testcase; std::cin >> testcase;
   while (testcase--) {
     int n; std::cin >> n;
     
     std::vector<std::pair<int,int>> t(n);
     std::vector<int> orig(n);
     for (int i=0; i<n; i++) {
       int t_i; std::cin >> t_i;
       t[i] = std::make_pair(t_i, i);
       orig[i] = t_i;
     }
     
     // defuse bombs in order
     std::sort(t.begin(), t.end());
     std::stack<std::pair<int,int>> s;
     
    for (int i=n-1; i>=0; i--) {
      s.push(t[i]);  
    }
    
    std::vector<bool> defused(n, false);
     
    int timestamp = 0;
    bool fail = false;
    
    while(s.size() > 0) {
      std::pair<int,int> curr = s.top();
      // std::cout << "considering bomb j=" << curr.second << " with t_j=" << curr.first << std::endl;
      
      if (defused[curr.second]) {
        s.pop();
        continue;
      }
      if (timestamp < curr.first) {
        if (2*curr.second + 2 < n) {
          int childLeft = 2*curr.second + 1;
          int childRight = 2*curr.second + 2;
          if (!defused[childLeft]) { // defuse l child first
            s.push(std::make_pair(orig[childLeft], childLeft));
          }
          if (!defused[childRight]) { // defuse r child first
            s.push(std::make_pair(orig[childRight], childRight));
          }
          if (defused[childLeft] && defused[childRight]) { // defuse curr
            defused[curr.second] = true;
            s.pop();
            timestamp++;
          }
        } else {
          // leaf node, defuse
          defused[curr.second] = true;
          s.pop();
          timestamp++;
        }
      } else { // fail
        fail = true;
        break;
      }
    }
    if (fail) std::cout << "no\n";
    else std::cout << "yes\n";
   }
 }
