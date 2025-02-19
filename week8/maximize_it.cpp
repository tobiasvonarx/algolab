#include <iostream>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int p;
  while (std::cin >> p) {
    if (p == 0) break;
    int a, b; std::cin >> a >> b;
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
    const int X = 0; 
    const int Y = 1;
    const int Z = 2;
    
    if (p == 1) {
      lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
      lp.set_a(X, 1, 4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b); // 4x + 2y <= ab
      lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x + y <= 1
      lp.set_l(X, true, 0); lp.set_l(Y, true, 0); // x, y >= 0
      lp.set_c(X, a); lp.set_c(Y, -b); // max by - ax
    } else {
      lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_b(0, 4); // x + y >= -4
      lp.set_a(X, 1, -4); lp.set_a(Y, 1, -2); lp.set_a(Z, 1, -1); lp.set_b(1, a*b); // 4x + 2y + z >= -ab
      lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_b(2, 1); // -x + y >= -1
      lp.set_u(X, true, 0); lp.set_u(Y, true, 0); lp.set_u(Z, true, 0); // x, y, z <= 0
      lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1); // min ax + by + z
    }
    
    Solution s = CGAL::solve_linear_program(lp, ET());

    if (s.is_optimal()) {
      if (p == 1) std::cout << -s.objective_value().numerator() / s.objective_value().denominator() << "\n";
      else std::cout << s.objective_value().numerator() / s.objective_value().denominator() << "\n";
    } else if (s.is_infeasible()) {
      std::cout << "no\n";
    } else if (s.is_unbounded()) {
      std::cout << "unbounded\n";
    }
  }
}