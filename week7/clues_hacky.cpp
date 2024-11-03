#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::pair<K::Point_2,Index> IPoint;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m; long r; std::cin >> n >> m >> r;
    long rr = r*r;
    
    graph G(n);
    
    // are there interferences?
    // construct graph and check if bipartite
    
    // store magnitude and coords of stations
    std::vector<std::tuple<double,long,long>> stations(n);
    std::vector<IPoint> pts(n);

    for (int i=0; i<n; i++) {
      long x, y; std::cin >> x >> y;
      stations[i] = std::make_tuple(std::sqrt(x*x + y*y), x, y);
    }
    
    std::sort(stations.begin(), stations.end());
    
    for (int i=0; i<n; i++) {
      pts[i] = std::make_pair(K::Point_2(std::get<1>(stations[i]), std::get<2>(stations[i])), i);
    }
    
    // if 2 stations differ more than r in their magnitude, stop
    for (int i=0; i<n; i++) {
      double dist = std::get<0>(stations[i]);
      long x = std::get<1>(stations[i]), y = std::get<2>(stations[i]);
      
      for (int j=i-1; j>=0; j--) {
        if (std::get<0>(stations[j]) < (dist - r)) break;
        long dx = std::get<1>(stations[j]) - x;
        long dy = std::get<2>(stations[j]) - y;
        if (dx*dx + dy*dy <= rr) {
          boost::add_edge(i, j, G);
        }
      }
    }
    
    bool noInterference = boost::is_bipartite(G);
    
    std::vector<int> component_map(n);
    boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

    // to figure out the closest station to the sender
    Triangulation t;
    t.insert(pts.begin(), pts.end()); 

    for (int i=0; i<m; i++) {
      long ax, ay, bx, by; std::cin >> ax >> ay >> bx >> by;
      if (!noInterference) {
        std::cout << "n";
      } else if ((ax-bx)*(ax-bx) + (ay-by)*(ay-by) <= rr) {
        std::cout << "y";
      } else {
        K::Point_2 a(ax, ay), b(bx, by);
        
        Triangulation::Vertex_handle nearestStationToA = t.nearest_vertex(a);
        Triangulation::Vertex_handle nearestStationToB = t.nearest_vertex(b);
        
        if (CGAL::squared_distance(nearestStationToA->point(), a) <= rr &&
            CGAL::squared_distance(nearestStationToB->point(), b) <= rr &&
            component_map[nearestStationToA->info()] == component_map[nearestStationToB->info()]) {
          std::cout << "y";
        } else {
          std::cout << "n";
        }
      }
    }
    std::cout << "\n";
    
  }
}
