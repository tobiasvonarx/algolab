#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int l, p; std::cin >> l >> p;
    
    graph G(l);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // circulation problem as in slides
    std::vector<long> supply(l);
    std::vector<long> demand(l);
    for (int i=0; i<l; i++) {
      std::cin >> supply[i] >> demand[i];
    }
    
    for (int i=0; i<p; i++) {
      int u, v; long minC, maxC;
      std::cin >> u >> v >> minC >> maxC;
      
      // enforce constraint of flow between minC and maxC as in circ. problem
      demand[u] += minC;
      supply[v] += minC;
      
      adder.add_edge(u, v, maxC-minC);
    }
    
    long totalDemand = 0;
    // edges from source to locations and locations to sink
    for (int i=0; i<l; i++) {
      totalDemand += demand[i];
      adder.add_edge(v_source, i, supply[i]);
      adder.add_edge(i, v_sink, demand[i]);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

    if (flow == totalDemand) std::cout << "yes\n";
    else std::cout << "no\n";
  }
}
