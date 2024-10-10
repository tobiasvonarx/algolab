#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

using namespace std;

int mst_weight(const weighted_graph &G, const weight_map &weights) {
  vector<edge_desc> mst; // store mst edges

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int mst_weight = 0;
  for (const auto e : mst) mst_weight += weights[e];
  
  return mst_weight;
}

int dijkstra_max_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int max_dist = 0;
  for (const auto &dist : dist_map) max_dist = max(dist, max_dist);

  return max_dist;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);
    
    for (int i=0; i<m; i++) {
      int u, v, weight; cin >> u >> v >> weight;
      edge_desc e = boost::add_edge(u, v, G).first;
      weights[e] = weight;
    }
    
    int mstWeight = mst_weight(G, weights);
    int maxDist = dijkstra_max_dist(G, 0);
    
    cout << mstWeight << " " << maxDist << "\n";
  }
}