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
  
  
  int n;
  while (std::cin >> n) {
    if (n == 0) break;

    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
    int d; std::cin >> d;
    const int r = d;

    for (int i=0; i<n; i++) {
      std::vector<int> a_i(d);
      long sqNorm = 0; // norm of a_i
      for (int j=0; j<d; j++) {
        // (a_i)_j
        std::cin >> a_i[j];
        sqNorm += a_i[j] * a_i[j];
      }
      int norm = std::sqrt(sqNorm);
      
      int b_i; std::cin >> b_i;
      
      // a_i^T x = b_i defines the border
      // a_i^T x = b_i - r defines border r closer to the center
      // normalize line eq. with norm of the coefficients to ensure arbitrary r not possible
      
      // a_i^T x / norm <= b_i / norm - r <=>
      // a_i^T x + r*norm <= b_i
      
      for (int j=0; j<d; j++) lp.set_a(j, i, a_i[j]);
      lp.set_a(r, i, norm);
      lp.set_b(i, b_i);
    }
    
    lp.set_l(r, true, 0);
    lp.set_c(r, -1); // max r
    
    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_optimal()) std::cout << -s.objective_value().numerator()/s.objective_value().denominator() << "\n";
    else if (s.is_infeasible()) std::cout << "none\n";
    else if (s.is_unbounded()) std::cout << "inf\n";
  }
}