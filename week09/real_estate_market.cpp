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
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; //

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
  while(testcase--) {
    int N, M, S; std::cin >> N >> M >> S;
    
    std::vector<int> limit(S);
    for (int i=0; i<S; i++) std::cin >> limit[i];
    
    std::vector<int> stateOfSite(M);
    for (int i=0; i<M; i++) {
      int state; std::cin >> state; // 1-indexed
      stateOfSite[i] = state-1;
    }
    
    graph G(N+M+S);
    edge_adder adder(G); 
    
    for (int buyer=0; buyer<N; buyer++) {
      for (int site=0; site<M; site++) {
        int bid; std::cin >> bid;
        int invBid = -bid + 100; // non-negative

        // edge between buyer and site with cap 1 and cost invBid
        adder.add_edge(buyer, N+site, 1, invBid);
      }
    }
    
    // edge from sites to state
    for (int site=0; site<M; site++) {
      adder.add_edge(N+site, N+M+stateOfSite[site], 1, 0);
    }
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // edge from source to buyer
    for (int buyer=0; buyer<N; buyer++) {
      adder.add_edge(v_source, buyer, 1, 0);
    }
    
    // edge from state to sink with limit as the capacity
    for (int state=0; state<S; state++) {
      adder.add_edge(N+M+state, v_sink, limit[state], 0);
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost = boost::find_flow_cost(G);
    int s_flow = 0;
    
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
    }
    
    // cost = sum (-bid + 100) <=> sum (bid) = 100*flow - cost
    std::cout << s_flow << " " << (100*s_flow)-cost << "\n";
  }
}
