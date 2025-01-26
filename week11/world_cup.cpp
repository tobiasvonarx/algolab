#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::pair<K::Point_2,Index> IPoint;

// bruh
double floor_to_double(const CGAL::Quotient<CGAL::Gmpz>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int testcase; std::cin >> testcase;
  while (testcase--) {
    int n, m, c; std::cin >> n >> m >> c;
    
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    auto amt = [&](int i, int j) {
      return i*m + j;
    };
    
    std::vector<int> alc(n);
    std::vector<IPoint> points;
    points.reserve(n+m);
    for (int i=0; i<n; i++) {
      int x, y, s; std::cin >> x >> y >> s >> alc[i];
      points.emplace_back(K::Point_2(x, y), i);
      // supply is not overshot
      for (int j=0; j<m; j++) {
        lp.set_a(amt(i,j), i, 1);
      }
      lp.set_b(i, s);
    }
    
    for (int j=0; j<m; j++) {
      int x, y, d, u; std::cin >> x >> y >> d >> u;
      points.emplace_back(K::Point_2(x, y), j+n);
      // demand exactly met
      for (int i=0; i<n; i++) {
        lp.set_a(amt(i,j), n+j, 1); // =
        lp.set_a(amt(i,j), n+m+j, alc[i]); // alcohol limit
      }
      lp.set_b(n+j, d); // demand
      lp.set_r(n+j, CGAL::EQUAL);

      lp.set_b(n+m+j, u*100); // sum/100 <= u <=> sum <= 100u
    }
    
    std::vector<std::vector<int>> revenues(n, std::vector<int>(m));
    for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        std::cin >> revenues[i][j];
        // lp.set_c(amt(i,j), -revenue);
      }
    }
    
    Triangulation t;
    t.insert(points.begin(), points.end());
    
    std::vector<std::vector<int>> contours(n+m, std::vector<int>());

    // reward term: (r_w,s − (t_w,s/100)) * amt_w,s
    // <=> (100*r_w,s - t_w,s) * amt_w,s
    for (int i=0; i<c; i++) {
      int x, y; long r; std::cin >> x >> y >> r;
      
      K::Point_2 P(x,y);
      
      // is stadium/warehouse within the circle defined by x,y,r?
      Triangulation::Vertex_handle closest = t.nearest_vertex(P);
      if (CGAL::squared_distance(closest->point(), P) < r*r) {
        for (int k=0; k<n+m; k++) {
          if (CGAL::squared_distance(points[k].first, P) < r*r) {
            contours[points[k].second].push_back(i);
          }
        }
      }
    }
    
    for (int i=0; i<n; i++) {
      for (int j=0; j<m; j++) {
        // t_w,s is the set diff
        int t = 0;
        for (int contour : contours[i]) {
          bool found = false;
          for (int contour_ : contours[j+n]) {
            if (contour == contour_) {
              found = true;
              break;
            }
          }
          if (!found) t++;
        }
        for (int contour : contours[j+n]) {
          bool found = false;
          for (int contour_ : contours[i]) {
            if (contour == contour_) {
              found = true;
              break;
            }
          }
          if (!found) t++;
        }
        
        // max (100*r_w,s - t_w,s) * amt_w,s
        lp.set_c(amt(i,j), t - 100*revenues[i][j]);
      }
    }
    
    
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_infeasible()) {
      std::cout << "no\n";
    } else {
      std::cout << (long)floor_to_double(-s.objective_value()/100) << "\n";
    }
    
  }
}
