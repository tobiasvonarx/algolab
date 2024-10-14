#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

using namespace std;

long round_up(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a < x) a += 1;
  return long(a);
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int n;
  while (cin >> n) {
    if (n == 0) break;
    vector<K::Point_2> P(n);
    
    for (int i = 0; i < n; ++i) {
      long x, y; cin >> x >> y;
      P[i] = K::Point_2(x, y);
    }
    
    Min_circle mc(P.begin(), P.end(), true);
    Traits::Circle c = mc.circle();
    cout << round_up(CGAL::sqrt(c.squared_radius())) << "\n";
  }
}