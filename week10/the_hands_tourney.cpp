#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, k, f0; long s0; std::cin >> n >> k >> f0 >> s0;
    std::vector<IPoint> points;
    points.reserve(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      points.emplace_back(K::Point_2(x, y), i);
    }
    
    Triangulation t;
    t.insert(points.begin(), points.end());
    
    EdgeV edges;
    edges.reserve(3*n); // there can be no more in a planar graph
    for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second+1)%3)->info();
      Index i2 = e->first->vertex((e->second+2)%3)->info();
      // ensure smaller index comes first
      if (i1 > i2) std::swap(i1, i2);
      edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    std::sort(edges.begin(), edges.end(),
        [](const Edge& e1, const Edge& e2) -> bool {
          return std::get<2>(e1) < std::get<2>(e2);
              });
    
    boost::disjoint_sets_with_storage<> uf(n);
    int numComponents = n;
    
    int f_ = 1;
    long s = -1;
    
    std::vector<int> componentMap(n, 1);
    std::vector<int> componentBuckets(5, 0);
    componentBuckets[1] = numComponents;
    
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      // determine components of endpoints
      Index family1 = uf.find_set(std::get<0>(*e));
      Index family2 = uf.find_set(std::get<1>(*e));

      int f = numComponents;
      if (k == 2) {
        int components2 = componentBuckets[2]+componentBuckets[3]+componentBuckets[4], components1 = componentBuckets[1];
        f = components2 + components1/2;
      } else if (k == 3) {
        int components3 = componentBuckets[3]+componentBuckets[4], components2 = componentBuckets[2], components1 = componentBuckets[1];
        int pairs = std::min(components2, components1);
        f = components3 + pairs + (components2-pairs)/2 + (components1-pairs)/3;
      } else if (k == 4) {
        int components4 = componentBuckets[4], components3 = componentBuckets[3], components2 = componentBuckets[2], components1 = componentBuckets[1];
        int pairs22 = components2/2;
        int pairs31 = std::min(components3, components1);

        components2 = components2%2;
        components1 = components1 - pairs31;
        components3 = components3 - pairs31;
        
        f = components4 + pairs22 + pairs31;

        // at this point: either threes left or ones left but not both
        if (components2 > 0) {
          if (components1 > 1) { // 2 1 1 and only 1s left now
            f++;
            components1 -= 2;
            components2--;
          } else if (components3 > 0) { // 2 3 and only 3s left now
            f++;
            components3--;
            components2--;
          } else {} // no 1,3,4 but still a 2 left, add it to any family
        }
        
        f += components1/4;
        f += components3/2;
      }
      
      if (std::get<2>(*e) >= s0 && f_ == 1) f_ = f;

      // if (numComponents == f0) s = (long)std::get<2>(*e);
      if (f >= f0) s = (long)std::get<2>(*e);
      
      if (family1 != family2) {
        uf.link(family1, family2);
        if (uf.find_set(std::get<0>(*e)) == family2) {
          componentBuckets[std::min(4,componentMap[family1])]--;
          componentBuckets[std::min(4,componentMap[family2])]--;
          
          componentMap[family2] += componentMap[family1];
          componentMap[family1] = 0;
          
          componentBuckets[std::min(4,componentMap[family2])]++;
        } else {
          componentBuckets[std::min(4,componentMap[family1])]--;
          componentBuckets[std::min(4,componentMap[family2])]--;
          
          componentMap[family1] += componentMap[family2];
          componentMap[family2] = 0;
          
          componentBuckets[std::min(4,componentMap[family1])]++;
        }
        numComponents--;
      }
    }
    std::cout << s << " " << f_ << "\n";
  }
}

