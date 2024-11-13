#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef long IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main() {
  std::ios_base::sync_with_stdio(false);
 
  int t; std::cin >> t;
  while (t--) {
    int n, m; long s; std::cin >> n >> m >> s;
    
    long sum_nobles_x = 0, sum_nobles_y = 0;
    std::vector<std::pair<long,long>> nobles(n);
    for (int i=0; i<n; i++) {
      long x, y; std::cin >> x >> y;
      nobles[i] = std::make_pair(x, y);
      sum_nobles_x += x;
      sum_nobles_y += y;
    }
    
    long sum_commons_x = 0, sum_commons_y = 0;
    std::vector<std::pair<long,long>> commons(m);
    for (int i=0; i<m; i++) {
      long x, y; std::cin >> x >> y;
      commons[i] = std::make_pair(x, y);
      sum_commons_x += x;
      sum_commons_y += y;
    }
    
    Program lp (CGAL::SMALLER, false, 0, false, 0); 
    
    const int M = 0;
    const int B = 1;
    const int MC = 2;
    const int T = 3;
    
    // Cersei constraint
    // y = -(1/m)x + c
    // my = -x + mc
    // => x_sewer = mc - m*y_house
    for (int i=0; i<n; i++) { // x_sewer - x_house >= 0
      lp.set_a(MC,i,-1); lp.set_a(M,i,nobles[i].second); lp.set_b(i,-nobles[i].first);
    }
    
    for (int i=0; i<m; i++) { // x_sewer - x_house <= 0
      lp.set_a(MC,n+i,1); lp.set_a(M,n+i,-commons[i].second); lp.set_b(n+i,commons[i].first);
    }
    
    Solution sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) {
      std::cout << "Y\n";
      continue;
    }
    
    if (s != -1) {
      // Tywin's constraint
      // sum_nobles (x - x_house) + sum_commons (x_house - x) <= s
      // <=> (n-m)MC + (sum_commons_y - sum_nobles_y)M <= s + sum_nobles_x - sum_commons_x
      lp.set_a(MC,n+m,n-m); lp.set_a(M,n+m,sum_commons_y-sum_nobles_y);
      lp.set_b(n+m,s+sum_nobles_x-sum_commons_x);
    }
    
    sol = CGAL::solve_linear_program(lp, ET());
    if (sol.is_infeasible()) {
      std::cout << "B\n";
      continue;
    }
    
    // minimize t for |y - house_y| <= t
    
    int offset = n+m+1;
    for (int i=0; i<n; i++) {
      // house_x*M + B - T <= house_y
      lp.set_a(M,offset+2*i,nobles[i].first); lp.set_a(B,offset+2*i,1);
      lp.set_a(T,offset+2*i,-1); lp.set_b(offset+2*i, nobles[i].second);
      
      // -house_x*M - B - T <= -house_y
      lp.set_a(M,offset+2*i+1,-nobles[i].first); lp.set_a(B,offset+2*i+1,-1);
      lp.set_a(T,offset+2*i+1,-1); lp.set_b(offset+2*i+1,-nobles[i].second);
    }
    
    offset = n+m+1 + (2*n);
    for (int i=0; i<m; i++) {
      // house_x*M + B - T <= house_y
      lp.set_a(M,offset+2*i,commons[i].first); lp.set_a(B,offset+2*i,1);
      lp.set_a(T,offset+2*i,-1); lp.set_b(offset+2*i, commons[i].second);
      
      // -house_x*M - B - T <= -house_y
      lp.set_a(M,offset+2*i+1,-commons[i].first); lp.set_a(B,offset+2*i+1,-1);
      lp.set_a(T,offset+2*i+1,-1); lp.set_b(offset+2*i+1,-commons[i].second);
    }
    
    lp.set_c(T, 1);  
    sol = CGAL::solve_linear_program(lp, ET());
    std::cout << (long)std::ceil(CGAL::to_double(sol.objective_value())) << "\n";
  }
}