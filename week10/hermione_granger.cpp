#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

typedef CGAL::Gmpq IT;
typedef CGAL::Gmpq ET;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Triangulation;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

struct Slytherin {
  int m;
  int c;
  int g;
};

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int testcase; std::cin >> testcase;
  while (testcase--) {
    int f, m, c, g; std::cin >> f >> m >> c >> g;
    int d, s; std::cin >> d >> s;
    
    std::vector<IPoint> pts;
    pts.reserve(s);
    std::vector<Slytherin> slytherins(s);
    for (int i=0; i<s; i++) {
      int x_i, y_i, m_i, c_i, g_i; std::cin >> x_i >> y_i >> m_i >> c_i >> g_i;
      pts.emplace_back(K::Point_2(x_i, y_i), i);
      slytherins[i] = {m_i, c_i, g_i};
    }
    
    
    Triangulation t;
    t.insert(pts.begin(), pts.end());
    
    // potion amounts non-negative
    Program lp (CGAL::SMALLER, true, 0, false, 0); 
    
    std::vector<int> minFperSlytherin(s, 1e9);
    std::vector<int> bestDAperSlytherin(s,-1);
    
    // when multiple DA can interrogate the same slytherin, it is always optimal
    // to only let the one with the lowest f_j interrogate
    for (int j=0; j<d; j++) {
      int x_j, y_j, f_j; std::cin >> x_j >> y_j >> f_j;
      // get nearest slytherin student
      Index slytherin = t.nearest_vertex(K::Point_2(x_j, y_j))->info();
      
      if (f_j < minFperSlytherin[slytherin]) {
        minFperSlytherin[slytherin] = f_j;
        bestDAperSlytherin[slytherin] = j;
      }
    }

    for (int i=0; i<s; i++) {
      if (bestDAperSlytherin[i] != -1) { // this slytherin can be interrogated
        int f_j = minFperSlytherin[i];
        int j = bestDAperSlytherin[i];
        
        // cap interrogation time to 24h
        // interrogation time = amt_j / f_j <= 24
        lp.set_u(j, true, 24*f_j);
        
        // this DA leaks slytherins[slytherin] stats per hour
        // leaks slytherin.{m,c,g}*amt_j/f_j for amt_j ml of the potion
        lp.set_a(j, 0, CGAL::Gmpq(-slytherins[i].m, f_j));
        lp.set_a(j, 1, CGAL::Gmpq(-slytherins[i].c, f_j));
        lp.set_a(j, 2, CGAL::Gmpq(-slytherins[i].g, f_j));
        
        // sum of amt needs to be <= f
        lp.set_a(j, 3, 1);
      } 
    }
    
    lp.set_b(0, -m); lp.set_b(1, -c); lp.set_b(2, -g);
    
    lp.set_b(3, f);

    Solution sol = CGAL::solve_linear_program(lp, ET());
    
    if (sol.is_optimal()) std::cout << "L\n";
    else std::cout << "H\n";
  }
}