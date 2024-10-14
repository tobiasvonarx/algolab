#include <iostream>
#include <iomanip>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

using namespace std;

double floor_to_double(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

int main() {
  ios_base::sync_with_stdio(false);
  cout << fixed << setprecision(0);
  
  int n;
  while (cin >> n) {
    if (n == 0) break;
    long x, y, a, b; cin >> x >> y >> a >> b;
    
    P origin(x,y);
    R ray(origin, P(a,b));
    
    bool intersect = false;
    S segmentToIntersection;

    vector<S> segments(n);
    for (int i=0; i<n; i++) {
      long r, s, t, u; cin >> r >> s >> t >> u;
      segments[i] = S(P(r,s), P(t,u));
    }   
   
    random_shuffle(segments.begin(), segments.end()); // against adversarial input order

    for (const auto &segment : segments) {
      result_of<K::Intersect_2(S,S)>::type o;
      if (!intersect && CGAL::do_intersect(segment, ray))
        o = CGAL::intersection(segment, ray);
      else if (intersect && CGAL::do_intersect(segment, segmentToIntersection))
        o = CGAL::intersection(segment, segmentToIntersection);
      else
        continue;
      
      if (const P* op = boost::get<P>(&*o)) {
        if (!intersect || CGAL::has_smaller_distance_to_point(origin, *op, segmentToIntersection.target()))
          segmentToIntersection = S(origin, *op);
      } else if (const S* os = boost::get<S>(&*o)) { // segment lies on the ray, check which endpoint hit first
        if (CGAL::has_smaller_distance_to_point(origin, os->source(), os->target())) {
          if (!intersect || CGAL::has_smaller_distance_to_point(origin, os->source(), segmentToIntersection.target()))
            segmentToIntersection = S(origin, os->source());
        } else {
          if (!intersect || CGAL::has_smaller_distance_to_point(origin, os->target(), segmentToIntersection.target()))
            segmentToIntersection = S(origin, os->target());
        }
      }
      intersect = true;
    }
    
    if (intersect) {
      P closestIntersectionPoint = segmentToIntersection.target();
      cout << floor_to_double(closestIntersectionPoint.x()) << " " << floor_to_double(closestIntersectionPoint.y()) << "\n";
    } else {
      cout << "no\n";
    }
  }
}