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
  }

  struct Polygon {
    std::vector< detail::Point > points;
  };
  std::istream& operator>>(std::istream& in, Polygon& point);
  std::ostream& operator<<(std::ostream& out, const Polygon& point);
  bool isPolygonEmpty(const Polygon& polygon);
}

#endif
