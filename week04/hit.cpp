#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int n;
  while (cin >> n) {
    if (n == 0) break;
    long x, y, a, b; cin >> x >> y >> a >> b;
    R ray = R(P(x,y), P(a,b));
    
    bool intersect = false;
    for (int i=0; i<n; i++) {
      long r, s, t, u; cin >> r >> s >> t >> u;
      S segment = S(P(r,s), P(t,u));
      if (!intersect && CGAL::do_intersect(segment, ray))
        intersect = true;
    }
    
    cout << (intersect?"yes":"no") << "\n";
  }
}