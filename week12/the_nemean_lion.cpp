#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#undef NDEBUG
#include <cassert>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Finite_faces_iterator Face_iterator;
typedef Triangulation::Face_circulator Face_circulator;
int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n; K::FT s; int h; std::cin >> n >> s >> h;
    
    std::vector<K::Point_2> pts(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      pts[i] = K::Point_2(x, y);
    }
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    K::FT d2 = -1;
    int a2 = 0;
    
    // count amount of min-length edges
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      K::FT l = t.segment(e).squared_length();
      if (l < d2 || d2 == -1) {
        d2 = l;
        a2 = 1;
      } else if (l == d2) {
        a2++;
      }
    }
    
    K::FT d3 = -1;
    int a3 = 0;
    // iterate through delaunay triangles and get amount of triangles with 
    // min-radius of circumcircle, i.e. the distance of the dual voronoi vertex to any point
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
      K::Point_2 p = t.dual(f);
      K::FT r = CGAL::squared_distance(p, f->vertex(0)->point());
      if (r < d3 || d3 == -1) {
        d3 = r;
        a3 = 1;
      } else if (r == d3) {
        a3++;
      }
    }
      
    std::vector<std::pair<K::FT, int>> bounds;
    bounds.reserve(3*n);
      
    int as = 0;
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      CGAL::Object o = t.dual(e);
      K::FT lb, ub;
      
      if (const K::Segment_2* s = CGAL::object_cast<K::Segment_2>(&o)) {
        // distances from endpoints of segment to delaunay vertex
        K::FT distU = CGAL::squared_distance(s->vertex(0), e->first->vertex((e->second + 1) % 3)->point());
        K::FT distV = CGAL::squared_distance(s->vertex(1), e->first->vertex((e->second + 1) % 3)->point());
        
        ub = std::max(distU, distV);
                      
        if (CGAL::do_intersect(*s, t.segment(e))) {
          lb = t.segment(e).squared_length()/4;
        } else {
          lb = std::min(distU, distV);
        }
      } else if (const K::Ray_2* r = CGAL::object_cast<K::Ray_2>(&o)) {
        ub = std::numeric_limits<long>::max();
        
        K::FT distU = CGAL::squared_distance(r->source(), e->first->vertex((e->second + 1) % 3)->point());
        
        if (CGAL::do_intersect(*r, t.segment(e))) {
          lb = t.segment(e).squared_length()/4;
        } else {
          lb = distU;
        }
      }
      if (lb <= s && s <= ub) as++;
      bounds.emplace_back(lb, 0);
      bounds.emplace_back(ub, 1);
    }
    
    std::sort(bounds.begin(), bounds.end());
    
    int amax = 0;
    int cnt = 0;
    for (const auto &[val, ub] : bounds) {
      if (ub) {
        cnt--;
      } else {
        cnt++;
        amax = std::max(cnt, amax);
      }
    }

    std::cout << a2 << " " << a3 << " " << as << " " << amax << "\n";
  }
}
