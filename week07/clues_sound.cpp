#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::pair<K::Point_2,Index> IPoint;

bool noInterference(graph &G, Triangulation &t, long rr) {
  for (Triangulation::Vertex_iterator v1=t.vertices_begin(); v1!=t.vertices_end(); v1++) {
    if (t.is_infinite(v1)) continue;
    Triangulation::Vertex_circulator v2 = t.incident_vertices(v1);
    do {
      if (!t.is_infinite(v2) && CGAL::squared_distance(v1->point(),v2->point()) <= rr) {
        boost::add_edge(v1->info(), v2->info(), G);
        // edge case: v1,v2,v3 are within <= r of each other but don't form a delaunay triangle
        Triangulation::Vertex_circulator v3 = t.incident_vertices(v2);
        do {
          if (!t.is_infinite(v3) && v3->info() != v1->info() && CGAL::squared_distance(v1->point(),v3->point()) <= rr && CGAL::squared_distance(v2->point(),v3->point()) <= rr) {
            return false;
          }
        } while (++v3 != t.incident_vertices(v2));
      }
    } while (++v2 != t.incident_vertices(v1));
  }
  return boost::is_bipartite(G);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m; long r; std::cin >> n >> m >> r;
    long rr = r*r;

    std::vector<IPoint> s(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      s[i] = std::make_pair(K::Point_2(x, y), i);
    }
    Triangulation t;
    t.insert(s.begin(), s.end());
    
    // construct delaunay graph and check if bipartite
    graph G(n);
    bool interference = !noInterference(G, t, rr);

    std::vector<int> component_map(n);
    boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 

    for (int i=0; i<m; i++) {
      int ax, ay, bx, by; std::cin >> ax >> ay >> bx >> by;
      if (interference) {
        std::cout << "n";
      } else {
        K::Point_2 a(ax, ay), b(bx, by);
        Triangulation::Vertex_handle stationA = t.nearest_vertex(a);
        Triangulation::Vertex_handle stationB = t.nearest_vertex(b);
        
        if (CGAL::squared_distance(a,b) <= rr) { // communicate directly
          std::cout << "y";
        } else if (CGAL::squared_distance(a, stationA->point()) <= rr && // holmes close enough to initial station
            CGAL::squared_distance(b, stationB->point()) <= rr && // watson close enough to initial station
            component_map[stationA->info()] == component_map[stationB->info()]) { // stations reachable
          std::cout << "y";
        } else {
          std::cout << "n";
        }
      }
      
    }
    std::cout << "\n";
    
  }
}
