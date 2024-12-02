#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it;
typedef boost::graph_traits<graph>::vertex_descriptor vertex_desc;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m; std::cin >> n >> m;
    
    graph G(n);
    edge_adder adder(G);  
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    std::vector<int> c(n-1);
    for (int i=0; i<n-1; i++) {
      std::cin >> c[i];
      adder.add_edge(i, i+1, c[i], 128);
    }
    
    adder.add_edge(v_source, 0, c[0], 0);
    adder.add_edge(n-1, v_sink, c[n-2], 0);
    
    for (int i=0; i<n-2; i++) {
      if (c[i] < c[i+1]) { // capacity increases, ability to increase flow
        adder.add_edge(v_source, i+1, c[i+1]-c[i], 128*(i+1));
      } else if (c[i] > c[i+1]) { // capacity decreases, ability to decrease flow
        adder.add_edge(i+1, v_sink, c[i]-c[i+1], 128*(n-i-2));
      }
    }
    
    for (int i=0; i<m; i++) {
      int a, b, d; std::cin >> a >> b >> d;
      adder.add_edge(a, b, 1, 128*(b-a)-d);
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
  
    int flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        flow += c_map[*e] - rc_map[*e];     
    }
    
    std::cout << 128*flow*(n-1)-cost << "\n";
  }
}
