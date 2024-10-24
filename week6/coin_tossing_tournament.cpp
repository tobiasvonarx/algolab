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
  
  int t; std::cin>>t;
  while (t--) {
    int n, m; std::cin >> n >> m;
    
    graph G(m+n);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // lower bound leaderboard already known
    std::vector<int> lb(n, 0); 
    int unknownResults = 0;

    for (int i=0; i<m; i++) {
      int a, b, c; std::cin >> a >> b >> c;
      if (c==1) {
        lb[a]++;
      } else if (c==2) {
        lb[b]++;
      } else {
        unknownResults++;
        
        // from match to each player
        adder.add_edge(i, m+a, 1L);
        adder.add_edge(i, m+b, 1L);
        
        // from source to match
        adder.add_edge(v_source, i, 1L);
      }
    }
    
    long sumMissing = 0;
    bool impossible = false;
    
    // from player to sink
    for (int i=0; i<n; i++) {
      long s_i; std::cin >> s_i;

      if (s_i - lb[i] < 0) impossible = true;
      
      sumMissing += s_i - lb[i];
      adder.add_edge(m+i, v_sink, s_i - lb[i]);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    
    if (impossible || flow != unknownResults || flow != sumMissing) std::cout << "no\n";
    else std::cout << "yes\n";
  }
}
