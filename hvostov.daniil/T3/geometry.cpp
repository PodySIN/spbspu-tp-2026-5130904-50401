#include "geometry.hpp"
#include "input.hpp"
#include <iostream>
#include <limits>
#include <iterator>
#include <algorithm>

void hvostov::detail::readPoints(std::istream& in, std::vector< Point >& pts, size_t n)
{
  if (n == 0 || !in) {
    return;
  }
  Point pt;
  in >> pt;
  if (in) {
    pts.push_back(pt);
    hvostov::detail::readPoints(in, pts, n - 1);
  }
}

std::istream& hvostov::detail::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in, true);
  if (!sentry) {
    return in;
  }
  if (in.peek() == '\n') {
    in.setstate(std::ios::failbit);
    return in;
  }
  using d_t = Delimiter;
  in >> d_t{'('} >> dest.x >> d_t{';'} >> dest.y >> d_t{')'};
  return in;
}

std::ostream& hvostov::detail::operator<<(std::ostream& out, const Point& dest)
{
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
}

std::istream& hvostov::operator>>(std::istream& in, Polygon& dest)
{
  dest.points.clear();
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  const std::streamsize max = std::numeric_limits< std::streamsize >::max();
  size_t size = 0;
  in >> size;

  if (!in || size < 3) {
    in.clear();
    in.ignore(max, '\n');
    return in;
  }

  std::vector< detail::Point > polygon;
  polygon.reserve(size);
  hvostov::detail::readPoints(in, polygon, size);

  if (polygon.size() != size) {
    in.clear();
    in.ignore(max, '\n');
    return in;
  }

  if (in.peek() != '\n' && in.peek() != EOF) {
    in.clear();
    in.ignore(max, '\n');
    return in;
  }

  dest.points = std::move(polygon);
  in.ignore(max, '\n');
  return in;
}

std::ostream& hvostov::operator<<(std::ostream& out, const Polygon& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard fmtguard(out);
  size_t size = dest.points.size();
  if (!size) {
    return out;
  }
  out << size << ' ';
  using oii_t = std::ostream_iterator< detail::Point >;
  std::copy_n(dest.points.begin(), size - 1, oii_t{out, " "});
  out << dest.points[size - 1];
  return out;
}

bool hvostov::isPolygonEmpty(const Polygon& polygon)
{
  return polygon.points.empty();
}
