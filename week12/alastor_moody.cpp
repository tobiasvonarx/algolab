#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct Edge {
  int u;
  int v;
  int c;
  int d;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m, s, p; std::cin >> n >> m >> s >> p;
    
    weighted_graph G_sp(n);
    weight_map weights = boost::get(boost::edge_weight, G_sp);
    edge_desc e;
    
    std::vector<Edge> edges;
    edges.reserve(m);
    for (int i=0; i<m; i++) {
      int u, v, c, d; std::cin >> u >> v >> c >> d;
      edges.push_back({u, v, c, d});
      e = boost::add_edge(u, v, G_sp).first; weights[e] = d;
    }
    
    std::vector<int> start_dist_map(n);
    std::vector<int> end_dist_map(n);
    
    boost::dijkstra_shortest_paths(G_sp, s,
    boost::distance_map(boost::make_iterator_property_map(
      start_dist_map.begin(), boost::get(boost::vertex_index, G_sp))));
    
    boost::dijkstra_shortest_paths(G_sp, p,
    boost::distance_map(boost::make_iterator_property_map(
      end_dist_map.begin(), boost::get(boost::vertex_index, G_sp))));
    
    graph G(n);
    edge_adder adder(G);
    
    for (const Edge &edge : edges) {
      if (start_dist_map[edge.u] + edge.d + end_dist_map[edge.v] == start_dist_map[p]) {
        // adge edge (u,v) with cap c
        adder.add_edge(edge.u, edge.v, edge.c);
      }
      if (start_dist_map[edge.v] + edge.d + end_dist_map[edge.u] == start_dist_map[p]) {
        // add edge (v,u) with cap c
        adder.add_edge(edge.v, edge.u, edge.c);
      }
    }
    
    std::cout << boost::push_relabel_max_flow(G, s, p) << "\n";
  }
}
