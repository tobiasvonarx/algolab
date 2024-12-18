// maximum independent set, on the bipartite graph using flow
#include <iostream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
typedef traits::vertex_descriptor vertex_desc;
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n; std::cin >> n;
    
    std::vector<std::pair<int,int>> white;
    white.reserve((n*n+1)/2);
    
    std::vector<std::pair<int,int>> black;
    black.reserve(n*n/2);
    
    for (int i=0; i<n; i++) {
      for (int j=0; j<n; j++) {
        int present; std::cin >> present;
        if (!present) continue;
        
        if ((i+j)%2 == 0) white.emplace_back(i, j);
        else black.emplace_back(i, j);
      }
    }
    
    graph G(white.size()+black.size());
    edge_adder adder(G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    
    for (int whiteIdx=0; whiteIdx<white.size(); whiteIdx++) {
      const auto &[i,j] = white[whiteIdx];
      for (int blackIdx=0; blackIdx<black.size(); blackIdx++) {
        const auto &[i_,j_] = black[blackIdx];
        // add edge if they interfere
        bool conflict = (i_==i-1 && j_==j-2) || (i_==i-1 && j_==j+2) ||
                        (i_==i+1 && j_==j-2) || (i_==i+1 && j_==j+2) ||
                        (i_==i-2 && j_==j-1) || (i_==i-2 && j_==j+1) ||
                        (i_==i+2 && j_==j-1) || (i_==i+2 && j_==j+1);
        if (conflict) {
          adder.add_edge(whiteIdx, blackIdx+white.size(), 1);
        }
      }
    }
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    // edge from source to white squares
    for (int whiteIdx=0; whiteIdx<white.size(); whiteIdx++) {
      adder.add_edge(v_source, whiteIdx, 1);
    }
    
    // edge from black squares to sink
    for (int blackIdx=0; blackIdx<black.size(); blackIdx++) {
      adder.add_edge(blackIdx+white.size(), v_sink, 1); 
    }
    
    boost::push_relabel_max_flow(G, v_source, v_sink);
  
    // BFS to find vertex set S
    std::vector<int> vis(white.size()+black.size()+2, false); // visited flags
    std::queue<int> Q; // BFS queue (from std:: not boost::)
    vis[v_source] = true; // Mark the source as visited
    Q.push(v_source);
    while (!Q.empty()) {
      const int u = Q.front();
      Q.pop();
      out_edge_it ebeg, eend;
      for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
        const int v = boost::target(*ebeg, G);
        // Only follow edges with spare capacity
        if (rc_map[*ebeg] == 0 || vis[v]) continue;
        vis[v] = true;
        Q.push(v);
      }
    }
    
    int maxKnights = 0;
    for (int i = 0; i < white.size(); ++i) {
      if (vis[i]) maxKnights++;
    }
    for (int i = 0; i < black.size(); ++i) {
      if (!vis[white.size()+i]) maxKnights++;
    }
    
    std::cout << maxKnights << "\n";
  }
}
