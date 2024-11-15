#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m; long s; int k; std::cin >> n >> m >> s >> k;
    
    std::vector<IPoint> trees(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      trees[i] = std::make_pair(K::Point_2(x, y), i);
    }
    
    std::vector<K::Point_2> bones(m);
    for (int i=0; i<m; i++) {
      int x, y; std::cin >> x >> y;
      bones[i] = K::Point_2(x, y);
    }
    
    Triangulation t;
    t.insert(trees.begin(), trees.end());
    
    EdgeV edges;
    edges.reserve(3*n + m);
    
    // nearest tree to bone and dist
    for (int i=0; i<m; i++) {
      Index tree = t.nearest_vertex(bones[i])->info();
      edges.emplace_back(n, tree, 4*CGAL::squared_distance(t.nearest_vertex(bones[i])->point(), bones[i]));
    }
    
    for (Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
      Index i1 = e->first->vertex((e->second+1)%3)->info();
      Index i2 = e->first->vertex((e->second+2)%3)->info();
      
      if (i1 > i2) std::swap(i1, i2);
      edges.emplace_back(i1, i2, t.segment(e).squared_length());
    }
    
    // smaller distance first
    std::sort(edges.begin(), edges.end(),
              [](const Edge& e1, const Edge& e2) -> bool {
                return std::get<2>(e1) < std::get<2>(e2);
              });
    
    boost::disjoint_sets_with_storage<> uf(n+1);

    std::vector<int> numBones(n, 0);
    
    // std::cout << "testcase, k=" << k << "\n";
    
    long minDistForKBones = -1;
    int sizeMaxComponent = 0;
    bool overshotS = false;
    
    // Kruskal type processing
    for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
      Index c1 = uf.find_set(std::get<0>(*e));
      Index c2 = uf.find_set(std::get<1>(*e));
      
      auto dist = std::get<2>(*e);
      
      if (dist > s) overshotS = true;
      
      if ((int)std::get<0>(*e) == n) { // edge from bone to closest tree
        numBones[c2]++;
        if (!overshotS) sizeMaxComponent = std::max(sizeMaxComponent, numBones[c2]);
        if (numBones[c2] >= k && minDistForKBones == -1) minDistForKBones = dist;
        // std::cout << "processing bone with dist " << dist << " to tree, that component has " << numBones[c2] << " bones\n";
      } else if (c1 != c2) { // between trees that arent connected yet
        int mergedBones = numBones[c1] + numBones[c2];
        uf.link(c1, c2);
        // std::cout << "merging components " << c1 << " (size " << numBones[c1] << ") and " << c2 << " (size " << numBones[c2] << ") to " << uf.find_set(std::get<0>(*e)) << " (size " << mergedBones << ")\n";
        numBones[uf.find_set(std::get<0>(*e))] = mergedBones;
        if (!overshotS) sizeMaxComponent = std::max(sizeMaxComponent, mergedBones);
        if (mergedBones >= k && minDistForKBones == -1) minDistForKBones = dist;
      }
      
      if (minDistForKBones != -1 && overshotS) break;
    }
    
    std::cout << sizeMaxComponent << " " << minDistForKBones << "\n";
  }
}
