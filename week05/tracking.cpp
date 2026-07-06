#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;

using namespace std;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m, k, x, y; cin >> n >> m >> k >> x >> y;
    
    weighted_graph G(n*(k+1));
    edge_desc e;
    weight_map weights = boost::get(boost::edge_weight, G);
    
    for (int i=0; i<m; i++) {
      int a, b, c, d; cin >> a >> b >> c >> d;
      // dijkstra where vertex is (actual vertex, #rivers so far)
      if (d) { // river edge
        e = boost::add_edge(a, b, G).first; weights[e] = c;
        e = boost::add_edge(b, a, G).first; weights[e] = c;
        for (int k_=1; k_<=k; k_++) {
          e = boost::add_edge(a+(n*k_), b+(n*k_), G).first; weights[e] = c;
          e = boost::add_edge(b+(n*k_), a+(n*k_), G).first; weights[e] = c;

          e = boost::add_edge(a+(n*(k_-1)), b+(n*k_), G).first; weights[e] = c;
          e = boost::add_edge(b+(n*(k_-1)), a+(n*k_), G).first; weights[e] = c;
        }
      } else {
        for (int k_=0; k_<=k; k_++) {
          e = boost::add_edge(a+(n*k_), b+(n*k_), G).first; weights[e] = c;
          e = boost::add_edge(b+(n*k_), a+(n*k_), G).first; weights[e] = c;
        }
      }
    }
    
    cout << dijkstra_dist(G, x, y+(n*k)) << "\n";
  }
}