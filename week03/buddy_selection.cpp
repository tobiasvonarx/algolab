#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

using namespace std;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

int main() {
  ios_base::sync_with_stdio(false);
  
  int t; cin >> t;
  while (t--) {
    int n, c, f; cin >> n >> c >> f;
    
    graph G(n);
    vector<set<string>> traits(n, set<string>());
    
    for (int i=0; i<n; i++) {
      for (int j=0; j<c; j++) {
        string s; cin >> s;
        traits[i].insert(s);
      }
    }
    
    // if intersection size > f, add edge
    for (int i=0; i<n; i++) {
      for (int j=0; j<i; j++) {
        vector<string> common_traits;
        set_intersection(traits[i].begin(), traits[i].end(),
                         traits[j].begin(), traits[j].end(),
                         back_inserter(common_traits));
        if (common_traits.size() > (long unsigned int)f)
          boost::add_edge(i, j, G);
      }
    }
    
    // check if matching is perfect
    if (maximum_matching(G)*2 == n) cout << "not optimal\n";
    else cout << "optimal\n";
  }
}