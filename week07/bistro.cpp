#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  while (std::cin >> n) {
    if (n==0) break;
    std::vector<K::Point_2> pts(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      pts[i] = K::Point_2(x, y);
    }
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    int m; std::cin >> m;
    std::vector<K::Point_2> candidates(m);
    for (int i=0; i<m; i++) {
      int x, y; std::cin >> x >> y;
      candidates[i] = K::Point_2(x, y);
    }
    
    for (const auto &cand : candidates) {
      long dist = CGAL::squared_distance(t.nearest_vertex(cand)->point(), cand);
      std::cout << dist << "\n";
    }
  }
}