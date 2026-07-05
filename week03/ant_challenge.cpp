#include <iostream>
#include <vector>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

typedef tuple<int,int,int> TII;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, e, s, a, b; cin >> n >> e >> s >> a >> b;
  
    vector<weighted_graph> G_local(s, weighted_graph(n));
    vector<weight_map> weightsLocal(s);
    for (int i=0; i<s; i++) weightsLocal[i] = boost::get(boost::edge_weight, G_local[i]);
    
    weighted_graph G_global(n);
    weight_map weightsGlobal = boost::get(boost::edge_weight, G_global);

    edge_desc ed;
    for (int i=0; i<e; i++) {
      int t1, t2; cin >> t1 >> t2;
      for (int j=0; j<s; j++) {
        int w_j; cin >> w_j;
        ed = boost::add_edge(t1,t2,G_local[j]).first;
        weightsLocal[j][ed] = w_j;
      }
    }
    
    vector<int> hives(s);
    for (int i=0; i<s; i++) cin >> hives[i];
    
    for (int species=0; species<s; species++) {
      // species specific MST
      vector<edge_desc> mst;
      boost::kruskal_minimum_spanning_tree(G_local[species], std::back_inserter(mst));
      
      for (const auto &it : mst) {
        int u = boost::source(it, G_local[species]);
        int v = boost::target(it, G_local[species]);
        
        if (!boost::edge(u,v,G_global).second) {
          ed = boost::add_edge(u,v,G_global).first;
          weightsGlobal[ed] = weightsLocal[species][it];
        } else {
          ed = boost::edge(u,v,G_global).first;
          if (weightsLocal[species][it] < weightsGlobal[ed])
            weightsGlobal[ed] = weightsLocal[species][it];
        }
      }
    }
    
    int timeNeeded = dijkstra_dist(G_global, a, b);
    cout << timeNeeded << "\n";
  }
}