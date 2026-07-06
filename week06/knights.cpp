#include <iostream>

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
    int m, n, k, c; std::cin >> m >> n >> k >> c;
    
    graph G(m*n*2);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // edges between intersections
    for (int i=0; i<m; i++) {
      for (int j=0; j<n; j++) {
        int curr_in = i*n + j;
        int curr_out = m*n + (i*n + j);
        if (i > 0) {
          // left out -> curr in
          int left_out = m*n + ((i-1)*n + j);
          adder.add_edge(left_out, curr_in, 1);
        } else {
          adder.add_edge(curr_out, v_sink, 1);
        }
        if (j > 0) {
          // top out -> curr in
          int top_out = m*n + (i*n + j - 1);
          adder.add_edge(top_out, curr_in, 1);
        } else {
          adder.add_edge(curr_out, v_sink, 1);
        }
        if (i < m-1) {
          // right out -> curr in
          int right_out = m*n + ((i+1)*n + j);
          adder.add_edge(right_out, curr_in, 1);
        } else {
          adder.add_edge(curr_out, v_sink, 1);
        }
        if (j < n-1) {
          // bottom out -> curr in
          int bottom_out = m*n + (i*n + j + 1);
          adder.add_edge(bottom_out, curr_in, 1);
        } else {
          adder.add_edge(curr_out, v_sink, 1);
        }
        
        // edge from in to out to restrict capacity of intersection to c
        adder.add_edge(curr_in, curr_out, c);
      }
    }
    
    for (int i=0; i<k; i++) {
      int x, y; std::cin >> x >> y;
      // edge from source to initial intersection of knight i
      adder.add_edge(v_source, x*n + y, 1);
    }
    
    long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
    std::cout << flow << "\n";
  }
}
