#include <iostream>

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
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
typedef traits::vertex_descriptor vertex_desc;

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
    int e, w, m, d; std::cin >> e >> w >> m >> d;
    int p, l; std::cin >> p >> l;
    
    graph G(2*(e+w));
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    const vertex_desc v_int_source = boost::add_vertex(G);
    const vertex_desc v_int_sink = boost::add_vertex(G);
    
    adder.add_edge(v_int_sink, v_sink, p-l*w, 0);
    adder.add_edge(v_source, v_int_source, p-l*e, 0);
    
    for (int u=0; u<e; u++) {
      adder.add_edge(v_source, u, l, 0);
      adder.add_edge(v_int_source, u, p-l*e, 0);
      adder.add_edge(v_int_source, e+w+u, p-l*e, 0);
    }
    
    for (int v=e; v<e+w; v++) {
      adder.add_edge(v, v_sink, l, 0);
      adder.add_edge(v, v_int_sink, p-l*w, 0);
      adder.add_edge(e+w+v, v_int_sink, p-l*w, 0);
    }

    for (int i=0; i<m; i++) {
      int u, v, r; std::cin >> u >> v >> r;
      adder.add_edge(u, e+v, 1, r);
    }
    
    for (int i=0; i<d; i++) {
      int u, v, r; std::cin >> u >> v >> r;
      adder.add_edge(e+w+u, e+w+e+v, 1, r);
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost2 = boost::find_flow_cost(G);

    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    int s_flow = 0;
    out_edge_it ed, eend;
    for (boost::tie(ed, eend) = boost::out_edges(boost::vertex(v_source,G), G); ed != eend; ++ed) {
      s_flow += c_map[*ed] - rc_map[*ed];     
    }

    if (s_flow < p || p < std::max(e*l,w*l)) {
      std::cout << "No schedule!\n";
    } else {
      std::cout << cost2 << "\n";
    }
  }
}
