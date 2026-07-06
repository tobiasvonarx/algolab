#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n;
  while(std::cin >> n) {
    if (n==0) break;
    
    int minx, miny, maxx, maxy; std::cin >> minx >> miny >> maxx >> maxy;
    
    std::vector<K::Point_2> pts(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      pts[i] = K::Point_2(x, y);
    }
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    std::vector<int> deathTimes;
    deathTimes.reserve(n);
    for (auto it=t.vertices_begin(); it!=t.vertices_end(); it++) { 
      if (t.is_infinite(it)) continue;
      //std::cout << "vertex " << it->point() << "\n";
      
      // (non-squared) distances
      int distWallLeft = it->point().x() - minx;
      int distWallRight = maxx - it->point().x();
      int distWallTop = it->point().y() - miny;
      int distWallBottom = maxy - it->point().y();
      
      int distWall = std::min(distWallLeft, std::min(distWallRight, std::min(distWallTop, distWallBottom)));
      int tWallDeath = std::ceil(std::sqrt(distWall - 0.5));
      
      // collide with closest bacteria
      long minDistToBacteria = std::numeric_limits<long>::max();
  
      int tDeath;

      Triangulation::Vertex_circulator v = t.incident_vertices(it);
      if (v != NULL) {
        do {
          if (!t.is_infinite(v)) {
            minDistToBacteria = std::min(minDistToBacteria, (long)CGAL::squared_distance(it->point(), v->point()));
          }
        } while (++v != t.incident_vertices(it));
        int tCollisionDeath = std::ceil(std::sqrt(std::sqrt(minDistToBacteria)/2 - 0.5));
        tDeath = std::min(tCollisionDeath, tWallDeath);
      } else {
        tDeath = tWallDeath;
      }
    
      deathTimes.push_back(tDeath);
    }
    
    std::sort(deathTimes.begin(), deathTimes.end());
    std::cout << deathTimes[0] << " " << deathTimes[n/2] << " " << deathTimes[n-1] << "\n";
  }
}