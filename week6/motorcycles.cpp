#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Ray_2 R;
typedef K::Point_2 P;
typedef K::Direction_2 D;

struct lt {
    inline bool operator() (const std::pair<R,int> &ray1, const std::pair<R,int> &ray2) {
        return (ray1.first.source().y() < ray2.first.source().y());
    }
};

D posDir(const D &orig) {
  if (orig.dy() >= 0) {
    return orig;
  } else {
    return D(orig.dx(), -orig.dy());
  }
}

D negDir(const D &orig) {
  if (orig.dy() <= 0) {
    return orig;
  } else {
    return D(orig.dx(), -orig.dy());
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) {
    int n; std::cin >> n;
    
    std::vector<std::pair<R,int>> rays(n);
    for (int i=0; i<n; i++) {
      long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
      rays[i] = std::make_pair(R(P(0, y0), P(x1, y1)), i);
    }
    
    std::sort(rays.begin(), rays.end(), lt());

    std::vector<bool> removedBikers(n, false);
    
    auto it = rays.begin();
    D best = D(it->first);
    D posBest = posDir(best);
    D negBest = negDir(best);

    for (it++; it != rays.end(); it++) {
      D curr(it->first);
      int currIdx = it->second;
      
      if (curr.counterclockwise_in_between(negBest, posBest) || curr == posBest) {
        best = curr;
        posBest = posDir(best);
        negBest = negDir(best);
      } else if (curr.dy()/curr.dx() < best.dy()/best.dx()) {
        removedBikers[currIdx] = true;
      }
    }

    auto rit = rays.rbegin();
    best = D(rit->first);
    posBest = posDir(best);
    negBest = negDir(best);

    for (rit++; rit != rays.rend(); rit++) {
      D curr(rit->first);
      int currIdx = rit->second;
      
      if (curr.counterclockwise_in_between(negBest, posBest) || curr == posBest || curr == negBest) {
        best = curr;
        posBest = posDir(best);
        negBest = negDir(best);
      } else if (curr.dy()/curr.dx() > best.dy()/best.dx()) {
        removedBikers[currIdx] = true;
      }
    }
    
    for (int i=0; i<n; i++) {
      if (!removedBikers[i]) std::cout << i << " ";
    }
    std::cout << "\n";
  }
}