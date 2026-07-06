#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int w, h; std::cin >> w >> h;
    
    std::vector<std::vector<int>> grid(h, std::vector<int>(w, -1));

    graph G(1);

    int vertexCnt = 0;
    for (int i=0; i<h; i++) {
      for (int j=0; j<w; j++) {
        char c; std::cin >> c;
        if (c == '.') {
          grid[i][j] = vertexCnt;
          
          if (i > 0 && grid[i-1][j] != -1) // up
            boost::add_edge(i*w+j, (i-1)*w+j, G);
          
          if (j > 0 && grid[i][j-1] != -1) // left
            boost::add_edge(i*w+j, i*w+j-1, G);
          
          vertexCnt++;
        }
      }
    }
    
    if (maximum_matching(G)*2 == vertexCnt) std::cout << "yes\n";
    else std::cout << "no\n";
  }
}