#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc; 
typedef boost::graph_traits<graph>::edge_iterator edge_it;

typedef std::size_t Index;
#include <boost/pending/disjoint_sets.hpp>

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, tatooine; std::cin >> n >> tatooine;
    
    std::vector<std::tuple<int,int,int>> edges;
    edges.reserve(n*(n-1)/2);
    for (int j=1; j<=n-1; j++) {
      for (int k=0; k<n-j; k++) {
        // channel between planets j and j+k
        int cost; std::cin >> cost;
        edges.emplace_back(cost, -(j+k), -(j-1));
      }
    }
    
    std::sort(edges.begin(), edges.end());
    
    boost::disjoint_sets_with_storage<> uf(n);
    int n_components = n;
    
    int mstCost = 0;

    std::vector<std::tuple<int,int,int>> remainingEdges;
    remainingEdges.reserve((n/2-1)*(n-1)); // 0.5*n*(n-1) - (n-1) = (0.5*n-1) * (n-1)
    
    std::vector<std::tuple<int,int,int>> mst;
    mst.reserve(n-1);
    
    graph G(n);
    
    for (const auto &[cost, negTo, negFrom] : edges) {
      Index c1 = uf.find_set(-negTo);
      Index c2 = uf.find_set(-negFrom);
      if (c1 != c2) {
        uf.link(c1, c2);
        mstCost += cost;
        boost::add_edge(-negFrom, -negTo, G);
        mst.emplace_back(cost, negTo, negFrom);
        --n_components;
      } else {
        remainingEdges.emplace_back(cost, negTo, negFrom);
      }
    }
    
    int best = std::numeric_limits<int>::max();
    for (const auto &[cost, negTo, negFrom] : mst) {
      boost::remove_edge(-negFrom, -negTo, G); // consider T \ edge
      
      std::vector<int> component_map(n);
      int ncc = boost::connected_components(G, boost::make_iterator_property_map(component_map.begin(), boost::get(boost::vertex_index, G))); 
      
      for (const auto &[cost_, negTo_, negFrom_] : remainingEdges) {
        if (component_map[-negFrom_] != component_map[-negTo_]) {
          int cand = mstCost - cost + cost_;
          best = std::min(best, cand);
          break;
        }
      }
      
      boost::add_edge(-negFrom, -negTo, G);
    }
    
    std::cout << best << "\n";
  }
}
