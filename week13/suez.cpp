#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#define int long

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
} 

signed main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m, h, w; std::cin >> n >> m >> h >> w;
    
    Program lp (CGAL::SMALLER, true, 1, false, 0); 
    
    std::vector<std::pair<int,int>> freeNails;
    freeNails.reserve(n);
    for (int i=0; i<n; i++) {
      int x, y; std::cin >> x >> y;
      freeNails.emplace_back(x, y);
    }
    
    std::vector<std::pair<int,int>> occupiedNails;
    occupiedNails.reserve(m);
    for (int i=0; i<m; i++) {
      int x, y; std::cin >> x >> y;
      occupiedNails.emplace_back(x, y);
    }
    
    int constraint = 0;
    for (int i=0; i<n; i++) {
      for (int j=i+1; j<n; j++) {
        // x + aw/2 <= x' - a'w/2 or y + ah/2 <= y' - a'h/2
        // i.e. a + a' <= max(2|x'-x|/w, 2|y'-y|/h)
        
        lp.set_a(i, constraint, 1);
        lp.set_a(j, constraint, 1);
        lp.set_b(constraint, 2*std::max(ET(std::abs(freeNails[i].first - freeNails[j].first),w), ET(std::abs(freeNails[i].second - freeNails[j].second),h)));
        constraint++;
      }
      lp.set_c(i, -2*(w+h)); // maximize 2*(w+h)*sum_i a_i
    }
    
    for (int i=0; i<n; i++) {
      // a' = 1
      ET minRHS;
      for (int j=0; j<m; j++) {
        ET candRHS = 2*std::max(ET(std::abs(freeNails[i].first - occupiedNails[j].first), w), ET(std::abs(freeNails[i].second - occupiedNails[j].second), h)) - 1;
        if (j == 0 || candRHS < minRHS) {
          minRHS = candRHS;
        }
      }
      
      if (m > 0) {
        lp.set_u(i, true, minRHS);
      }
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());

    std::cout << (long)-floor_to_double(s.objective_value()) << "\n";
    
  }
}
