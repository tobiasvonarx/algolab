#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<boost::graph_traits<graph>::vertex_descriptor> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  return boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m, b, p; long d; 
    std::cin >> n >> m >> b >> p >> d;
    
    std::vector<int> barracks(b);
    for (int i=0; i<b; i++) std::cin >> barracks[i];
    
    std::vector<bool> isPlaza(n, false);
    for (int i=0; i<p; i++) {
      int p_i; std::cin >> p_i;
      isPlaza[p_i] = true;
    }
    
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    edge_desc e;
  
    std::vector<std::pair<int,int>> roads(m);
    for (int i=0; i<m; i++) {
      int x, y; long l; std::cin >> x >> y >> l;
      roads[i] = std::make_pair(x, y);
      e = boost::add_edge(x, y, G).first; weights[e] = l;
    }

    std::vector<bool> reachable(n, false);
    
    for (const int &barrack : barracks) {
      std::vector<int> dist_map(n);
      boost::dijkstra_shortest_paths(G, barrack,
        boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));
          
      for (int i=0; i<n; i++) {
        if (dist_map[i] <= d) reachable[i] = true;
      }
    }
    
    // if both endpoints of road are reachable, add edge
    graph G_reachable(2*n);
    
    // for plazas, double the vertex
    for (const auto &road : roads) {
      if (reachable[road.first] && reachable[road.second]) {
        if (isPlaza[road.first]) {
          boost::add_edge(n+road.first, road.second, G_reachable);
        }
        if (isPlaza[road.second]) {
          boost::add_edge(road.first, n+road.second, G_reachable);
        }
        boost::add_edge(road.first, road.second, G_reachable);
      } 
    }
    
    std::cout << maximum_matching(G_reachable) << "\n";
  }
}
