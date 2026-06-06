#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iosfwd>
#include <vector>
#include <functional>
#include "geometry.hpp"
#include <unordered_map>
#include <vector>

namespace hvostov {
  namespace detail {
    struct SeqState {
      size_t current;
      size_t max;
    };

    double triangleArea(const std::vector< Point >& pts, size_t i);
    double getArea(const Polygon& p);
    bool isEvenVertexes(const Polygon& p);
    bool isOddVertexes(const Polygon& p);
    bool hasVertexesCount(size_t n, const Polygon& p);
    size_t getVertexesCount(const Polygon& p);
    int getX(const Point& p);
    int getY(const Point& p);
    bool isPointInBoundingBox(const Point& p, int minX, int minY, int maxX, int maxY);
    bool polygonsEqual(const Polygon& a, const Polygon& b);
    SeqState updateSeqState(SeqState state, bool match);
    double sumAreasOfFiltered(const std::vector< Polygon >& data, std::function< bool(const Polygon&) > pred);

    bool pointEqual(const Point& p1, const Point& p2);
    int getPolygonMinX(const Polygon& p);
    int getPolygonMaxX(const Polygon& p);
    int getPolygonMinY(const Polygon& p);
    int getPolygonMaxY(const Polygon& p);

  }

  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void inFrame(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxSeq(std::istream&, std::ostream&, const std::vector< Polygon >&);

  using cmd_t = std::function< void(std::istream&, std::ostream&, const std::vector< hvostov::Polygon >&) >;
  using data_t = std::vector< hvostov::Polygon >;
  void process(std::istream& in, std::ostream& out, std::unordered_map< std::string, cmd_t >& cmds, data_t data);
}

#endif
