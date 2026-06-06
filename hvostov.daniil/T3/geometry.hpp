#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <istream>
#include <vector>

namespace hvostov {
  namespace detail {
    struct Point {
      int x, y;
    };
    std::istream& operator>>(std::istream& in, Point& point);
    std::ostream& operator<<(std::ostream& out, const Point& point);
    void readPoints(std::istream& in, std::vector< Point >& pts, size_t n);
  }

  struct Polygon {
    std::vector< detail::Point > points;
  };
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  std::ostream& operator<<(std::ostream& out, const Polygon& polygon);
  bool isPolygonEmpty(const Polygon& polygon);
}

#endif
