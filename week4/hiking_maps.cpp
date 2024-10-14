// precompute which legs each t_i covers
// - O(nm) * O(point in triangle inclusion check)
// - determine if both endpoints of leg are inside triangle
// - store leg indices covered by t_i in vector covers[i]
// sliding window over t_i's with
// - state = map from leg index -> amt. of times leg is covered
// - if all legs are covered at least once, valid sol. candidate

#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int testcase; cin >> testcase;
  while (testcase--) {
    int m, n; cin >> m >> n;
    
    vector<P> path(m);
    
    for (int i=0; i<m; i++) {
      int x, y; cin >> x >> y;
      path[i] = P(x,y);
    }
    
    vector<vector<int>> covers(n, vector<int>());
    
    for (int i=0; i<n; i++) {
      int x, y;
      cin >> x >> y; P q0(x, y);
      cin >> x >> y; P q1(x, y);
      cin >> x >> y; P q2(x, y);
      cin >> x >> y; P q3(x, y);
      cin >> x >> y; P q4(x, y);
      cin >> x >> y; P q5(x, y);

      L line1(q0,q1); L line2(q2,q3); L line3(q4,q5);
      
      auto o1 = CGAL::intersection(line1, line2);
      const P* v1 = boost::get<P>(&*o1);
      
      auto o2 =  CGAL::intersection(line1, line3);
      const P* v2 = boost::get<P>(&*o2);

      auto o3 =  CGAL::intersection(line2, line3);
      const P* v3 = boost::get<P>(&*o3);
      
      vector<bool> triangleIntersections(m);
      
      for (int j=0; j<m; j++) {
        auto orientation1 = CGAL::orientation(*v1, *v2, path[j]);
        auto orientation2 = CGAL::orientation(*v2, *v3, path[j]);
        auto orientation3 = CGAL::orientation(*v3, *v1, path[j]);
        triangleIntersections[j] = (orientation1 <= 0 && orientation2 <= 0 && orientation3 <= 0) || (orientation1 >= 0 && orientation2 >= 0 && orientation3 >= 0);
      }
      
      for (int j=0; j<m-1; j++) {
        if (triangleIntersections[j] && triangleIntersections[j+1]) {
          covers[i].push_back(j);
        }
      }
    }
    
    // sliding window
    vector<int> counts(m-1,0); // how many times each path segment is covered
    int mnSol = 1e9;
    
    int left = 0, right = 0;
    for (const auto &j : covers[left]) counts[j]++;

    while (left < n && right < n) {
      bool ok = true;
      for (int j=0; j<m-1; j++) {
        if (counts[j] < 1) {
          ok = false;
          break;
        }
      }
      if (ok) { // candidate sol
        //cout << "found cand. sol with e-b=" << right+1 << "-" << left << "=" << right+1-left << "\n";
        if (right+1-left < mnSol) mnSol = right+1-left;
        for (const auto &j : covers[left]) counts[j]--;
        left++;
      } else {
        right++;
        if (right < n) {
          for (const auto &j : covers[right]) counts[j]++;
        }     
      }
    }

    cout << mnSol << "\n";
  }
}

