#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/biconnected_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>> graph;
typedef boost::graph_traits<graph>::edge_descriptor edge_desc; 
typedef boost::property_map<graph, boost::edge_weight_t>::type edge_map;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

using namespace std;

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    
    graph G(n);
    
    for (int i=0; i<m; i++) {
      int u, v; cin >> u >> v;
      boost::add_edge(u, v, G);
    }
    
    edge_map component = boost::get(boost::edge_weight, G);
    int ncc = boost::biconnected_components(G, component); 

    vector<vector<edge_desc>> componentEdges(ncc,vector<edge_desc>());
    
    edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::edges(G); ebeg != eend; ++ebeg) {
      // cout << *ebeg << " " << component[*ebeg] << "\n";
      componentEdges[component[*ebeg]].push_back(*ebeg);
    }
    
    set<pair<int,int>> bridges;
    for (const auto &edges : componentEdges) {
      if (edges.size() == 1) {
        int u = boost::source(edges[0], G);
        int v = boost::target(edges[0], G);
        
        bridges.insert(make_pair(min(u,v),max(u,v)));
      }
    }
    
    cout << bridges.size() << "\n";
    for (const auto &bridge : bridges) {
      cout << bridge.first << " " << bridge.second << "\n";
    }

  }
}