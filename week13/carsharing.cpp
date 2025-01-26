#include <iostream>
#include <vector>
#include <map>

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

struct Request {
  int s, t, d, a, p;
};

signed main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int N, S; std::cin >> N >> S;
    
    std::vector<int> l(S);
    for (int i=0; i<S; i++) std::cin >> l[i];
    
    std::vector<Request> requests;
    requests.reserve(N);
    
    std::map<std::pair<int, int>, int> m; // (station, time) -> index
    std::set<int> timesteps;
    
    std::map<int,int> offsets;
  
    int j = 0;
    for (int i=0; i<N; i++) {
      int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
      s--; t--;
      requests.push_back({s,t,d,a,p});
      if (m.find({s,d}) == m.end()) {
        m.insert({{s,d}, j++});
      }
      if (m.find({t,a}) == m.end()) {
        m.insert({{t,a}, j++});
      }
      
      timesteps.insert(d);
      timesteps.insert(a);
    }
    
    int offset = 0;
    for (int t: timesteps) {
      offsets[t] = offset++;
    }
    int mxOffset = offset-1;
    
    // for (auto x: offsets) std::cout << x.first << "," << x.second << " ";
    // std::cout << "\n";
    
    graph G(j);
    edge_adder adder(G);
    
    const vertex_desc v_source = boost::add_vertex(G);
    const vertex_desc v_sink = boost::add_vertex(G);
    
    int prevS = -1;
    int prevT = -1;
    int prevIdx = -1;
    for (auto it=m.begin(); it!=m.end(); it++) {
      int station = it->first.first;
      int timestamp = it->first.second;
      int index = it->second;
      
      // std::cout << station << "," << timestamp << " -> " << index << "\n";
      
      if (prevS == station) { 
        adder.add_edge(prevIdx, index, 1e9, 100*(offsets[timestamp]-offsets[prevT])); // connect s,t -> s,t' where t'>t
        // std::cout << prevIdx << " to " << index << " cost " << offsets[timestamp]-offsets[prevT] << "\n";
      } else { // connect source to station with initial car amount
        adder.add_edge(v_source, index, l[station], 100*offsets[timestamp]);
        // std::cout << "intiial offset " << offsets[timestamp] << "\n";
        if (prevIdx != -1) {
          adder.add_edge(prevIdx, v_sink, 1e9, 100*(mxOffset-offsets[prevT])); // connect prev station to sink
        }
      }
      
      prevS = station;
      prevT = timestamp;
      prevIdx = index;
    }
    adder.add_edge(prevIdx, v_sink, 1e9, 100*(mxOffset-offsets[prevT])); // connect last station to sink

    // process bookings
    for (const Request r : requests) {
      adder.add_edge(m[{r.s,r.d}], m[{r.t,r.a}], 1, 100*(offsets[r.a]-offsets[r.d])-r.p);
    }
    
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
    int cost2 = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    auto c_map = boost::get(boost::edge_capacity, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];     
    }

    std::cout << 100*s_flow*(timesteps.size()-1)-cost2 << "\n";
  }
}
