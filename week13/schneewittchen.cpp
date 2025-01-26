#include <iostream>
#include <vector>
#include <functional>
#include <queue>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#undef NDEBUG
#include <cassert>

#include <iomanip>
//print lp
template<typename T>
std::ostream& operator<<(std::ostream &os, const CGAL::Quadratic_program<T> &lp) {
    os << "minimize:\n";
    bool before = false;
    for(int j=0; j<lp.get_n(); j++) {
        T val = *(lp.get_c()+j);
        if(val != 0) {
            if(before)
                os << " + ";
            os << val << " * x" << j;
            before = true;
        }
    }
    if(lp.get_c0() != 0) {
        os << " + " << lp.get_c0();
    }
    os << std::endl << "where\n";
    for(int i=0; i<lp.get_m(); i++) {
        for(int j=0; j<lp.get_n(); j++) {
            os << std::setw(4) << *(*(lp.get_a()+j)+i);
        }
        os << std::setw(4) << "|";
        auto rel = *(lp.get_r()+i);
        os << std::setw(4) << ((rel == CGAL::SMALLER) ? "<=" : ((rel == CGAL::LARGER) ? ">=" : "==")) << " ";
        os << *(lp.get_b()+i);
        os << std::endl;
    }
    return os;
}


typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void solve() {
  int n, m; std::cin >> n >> m;
  
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  std::vector<std::vector<int>> r(n, std::vector<int>(m));
  std::vector<int> dangerous(n, -1);
  std::vector<int> dangerLevels;
  dangerLevels.reserve(21);
  dangerLevels[0] = 0; // doesnt matter
  dangerous[0] = 0;
  
  int k = 0;
  for (int i=0; i<n; i++) {
    int d; std::cin >> d;
    for (int j=0; j<m; j++) std::cin >> r[i][j];
    if (d >= 0) {
      dangerous[i] = ++k;
      dangerLevels[k] = d;
    }
  }
  
  std::vector<std::vector<int>> adj(n, std::vector<int>());
  for (int i=0; i<n-1; i++) {
    int u, v; std::cin >> u >> v;
    adj[v].push_back(u);
  }
  
  // rec(u) = how much of each mineral we can get in the subtree rooted at u
  // - raw amounts of each mineral
  // - vector of dangerous mines in the immediate path
  
  // rec(dangerous) induces constraints
  // for each j: 2*outgoing_j <= incoming_j = (sum_j mineral_j + sum_dangerous variable_i,j)
  // sum_j 2*outgoing_j <= threshold
  
  auto out = [&](int i, int j) {
    return i*m + j;
  };
  
  int constraint = 0;
  std::function<void(int)> rec = [&](int u) {
    std::vector<int> minerals(m);
    for (int i=0; i<m; i++) minerals[i] = r[u][i];
    std::vector<int> dangerousMines;

    std::queue<int> q;
    for (int v : adj[u]) q.push(v);
    
    while (q.size()) {
      int v = q.front();
      q.pop();
      if (dangerous[v] != -1) {
        dangerousMines.push_back(dangerous[v]);
        rec(v);
      } else {
        for (int j=0; j<m; j++) minerals[j] += r[v][j];
        for (int v_ : adj[v]) q.push(v_);
      }
    }
    
    assert(dangerous[u] != -1);
    if (dangerous[u]) {
      // for each j: 2*outgoing_j <= incoming_j = (mineral_j + sum_dangerous variable_i,j) 
      for (int j=0; j<m; j++) {
        // 2*out_k_j + sum_k' (-out_k',j)<= mineral_j
        lp.set_a(out(dangerous[u], j), constraint, 2);
        for (int k_ : dangerousMines) lp.set_a(out(k_, j), constraint, -1);
        lp.set_b(constraint, minerals[j]);
        constraint++;
      }
  
      // sum_j 2*outgoing_j <= threshold
      lp.set_b(constraint, dangerLevels[dangerous[u]]);
      for (int j=0; j<m; j++) lp.set_a(out(dangerous[u], j), constraint, 2);
      constraint++;
    } else {
      assert(dangerous[u] == 0);
      // out_k_j + sum_k' (-out_k',j) = mineral_j
      for (int j=0; j<m; j++) {
        lp.set_a(out(0,j), constraint, 1);
        for (int k_ : dangerousMines) lp.set_a(out(k_, j), constraint, -1);
        lp.set_b(constraint, minerals[j]);
        lp.set_r(constraint, CGAL::EQUAL);
        constraint++;
      }
    }
  };
  
  rec(0);
  
  
  int variable = (k+1)*m;
  for (int j=0; j<m; j++) {
    int c, s, p; std::cin >> c >> s >> p;
    
    // out(0,j) + amt_bought_j >= c
    lp.set_a(out(0,j), constraint, 1);
    lp.set_a(variable, constraint, 1);
    lp.set_b(constraint, c);
    lp.set_r(constraint, CGAL::LARGER);
    constraint++;
    
    // amt_bought_j <= s;
    lp.set_a(variable, constraint, 1);
    lp.set_b(constraint, s);
    constraint++;
    
    // minimize cost
    lp.set_c(variable, p);
    variable++;
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  // std::cout << lp;
  
  if (s.is_infeasible()) std::cout << "Impossible!\n";
  else std::cout << (long)CGAL::to_double(s.objective_value()) << "\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while (t--) solve();
}
