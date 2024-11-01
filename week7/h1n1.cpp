#include <iostream>
#include <vector>
#include <map>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Face_iterator  Face_iterator;
typedef Triangulation::Face_handle  Face_handle;

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
    
    // widestPossible[face] = w means that we can reach face from the outside with a disk of diameter^2 <= w
    std::unordered_map<Face_handle, long> widestPossible;
    std::queue<Face_handle> q;
    
    Triangulation::Face_circulator f = t.incident_faces(t.infinite_vertex());
    do {
      widestPossible[f] = std::numeric_limits<long>::max();
      q.push(f);      
    } while (++f != t.incident_faces(t.infinite_vertex()));
    
    for (Face_iterator f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f) {
      widestPossible[f] = 0;
    }
    
    // traversing adjacent delauney faces <=> going along voronoi edges
    while (q.size() > 0) {
      Face_handle f = q.front();
      q.pop();
      
      // neighbor i
      for (int i=0; i<3; i++) {
        Face_handle neighbor = f->neighbor(i);
        if (t.is_infinite(neighbor)) continue;
        // std::cout << "neighbor " << i << " is not infinite\n";
        long neighborWidth = CGAL::squared_distance(f->vertex((i+1)%3)->point(), f->vertex((i+2)%3)->point());
        long newWidth = std::min(widestPossible[f], neighborWidth);
        if (newWidth > widestPossible[neighbor]) {
          widestPossible[neighbor] = newWidth;
          q.push(neighbor);
          // std::cout << "exploring neighbor " << i << ", new width is " << newWidth << "\n";
        }
      }
    }
    
    int m; std::cin >> m;
    for (int i=0; i<m; i++) {
      int x, y; long d; std::cin >> x >> y >> d;
      K::Point_2 P(x, y);
      
      bool tooCloseInitially = CGAL::squared_distance(P, t.nearest_vertex(P)->point()) < d;
      
      Face_handle f = t.locate(P);
      bool cantEscape = widestPossible[f] < 4 * d;
      
      if (tooCloseInitially || cantEscape) std::cout << "n";
      else std::cout << "y";
    }
    std::cout << "\n";
  }
}
