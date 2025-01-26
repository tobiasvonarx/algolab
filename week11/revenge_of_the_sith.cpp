#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc; 
typedef boost::graph_traits<graph>::edge_iterator edge_it;

signed main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n; long r; std::cin >> n >> r;
    long rr = r*r;
    
    std::vector<std::pair<int,int>> allPoints;
    allPoints.reserve(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      allPoints.emplace_back(x, y);
    }
    
    int res = 0;
    int low = 0, high = n/2;
    
    while (low <= high) {
      int k = (low+high)/2;

      // ignore first k planets
      std::vector<IPoint> points;
      points.reserve(n-k);
      for (int i=0; i<n-k; i++) {
        points.emplace_back(K::Point_2(allPoints[i+k].first, allPoints[i+k].second), i);
      }
      
      Triangulation t;
      t.insert(points.begin(), points.end());
      
      // std::cout << "k=" << k << ", low=" << low << ", high=" << high << std::endl;
      graph G(n-k);
      for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
        int i1 = e->first->vertex((e->second+1)%3)->info();
        int i2 = e->first->vertex((e->second+2)%3)->info();
        if (t.segment(e).squared_length() <= rr) {
          boost::add_edge(i1, i2, G);
        }
      }
      
      std::vector<int> component_map(n-k);
      int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
      
      std::vector<int> component_vertices_cnt(ncc,0);
      for (int i=0; i<n-k; i++) component_vertices_cnt[component_map[i]]++;
      
      int mx = 0;
      for (int cc=0; cc<ncc; cc++) {
        mx = std::max(mx, component_vertices_cnt[cc]);
      }

      if (mx > k) { // increase k
        low = k + 1;
        res = std::max(res, k);
      } else if (mx < k) { // decrease k
        high = k - 1;
        res = std::max(res, mx);
      } else {
        res = mx;
        break;
      }
    }
    std::cout << res << "\n";
  }
}
