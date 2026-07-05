#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int n, m;
  while (std::cin >> n >> m) {
    if (n == 0 && m == 0) break;
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    std::vector<std::pair<int,int>> nutrients(n);
    
    for (int i=0; i<n; i++) {
      int lb, ub; std::cin >> lb >> ub;
      nutrients[i] = std::make_pair(lb, ub);
    }
    
    std::vector<int> prices(m);
    std::vector<std::vector<int>> foods(m, std::vector<int>(n));
    for (int j=0; j<m; j++) {
      std::cin >> prices[j];
      for (int i=0; i<n; i++) std::cin >> foods[j][i];
    }
    
    // constraints
    for (int i=0; i<n; i++) {
      // constrain amount of nutrient i to be in given range
      for (int j=0; j<m; j++) {
        // amount of nutrient i in food j
        lp.set_a(j, i, foods[j][i]);
        lp.set_a(j, i+n, -foods[j][i]);
      }
      
      lp.set_b(i, nutrients[i].second); // upper bound
      lp.set_b(i+n, -nutrients[i].first); // lower bound
    }
    
    // min the cost, elementwise amt * price
    for (int j=0; j<m; j++) lp.set_c(j, prices[j]);

    Solution s = CGAL::solve_linear_program(lp, ET());
    
    if (s.is_optimal()) {
      std::cout << s.objective_value().numerator()/s.objective_value().denominator() << "\n";
    } else if (s.is_infeasible()) {
      std::cout << "No such diet.\n";
    }
  }
}