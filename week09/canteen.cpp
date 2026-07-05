#include <iostream>
#include <vector>

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
typedef traits::vertex_descriptor vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; 

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
    int n; std::cin >> n;
    
    std::vector<int> a(n);
    std::vector<int> c(n);
    for (int i=0; i<n; i++) std::cin >> a[i] >> c[i];
    
    std::vector<int> s(n);
    std::vector<int> p(n);
    int totalStudents = 0;
    for (int i=0; i<n; i++) {
      std::cin >> s[i] >> p[i];
      totalStudents += s[i];
    }
    
    std::vector<int> e(n-1);
    std::vector<int> v(n-1);
    for (int i=0; i<n-1; i++) std::cin >> v[i] >> e[i];
    
    graph G(n);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);

    // edge from production to sink, per day
    for (int i=0; i<n; i++) {
      adder.add_edge(i, v_sink, s[i], -p[i]+20);
    }
    
    // edge for storing/stored meals for the night
    for (int i=0; i<n-1; i++) {
      adder.add_edge(i, i+1, v[i], e[i]);
    }
    
    // source to production
    for (int i=0; i<n; i++) {
      adder.add_edge(v_source, i, a[i], c[i]);
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    
    int flow = 0;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    out_edge_it edge, eend;
    for(boost::tie(edge, eend) = boost::out_edges(boost::vertex(v_source,G), G); edge != eend; ++edge) {
        flow += c_map[*edge] - rc_map[*edge];     
    }
    
    if (flow == totalStudents) std::cout << "possible ";
    else std::cout << "impossible ";
    
    // add back the 20 revenue per fed student which
    // we lost by making the menu 20 cheaper for non-negativity
    int actualCost = flow*20 - cost;
    
    std::cout << flow << " " << actualCost << "\n";
  }
}
