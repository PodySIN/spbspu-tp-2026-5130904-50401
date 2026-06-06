#include "geometry.hpp"
#include <algorithm>
#include <iostream>
#include <iterator>

#include "input.hpp"

std::istream& hvostov::detail::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard fmtguard(in);
  using d_t = Delimiter;
  detail::Point p{0, 0};
  in >> d_t{'('} >> p.x >> d_t{';'} >> p.y >> d_t{')'};
  if (in) {
    dest = p;
  }
  return in;
}

std::ostream& hvostov::detail::operator<<(std::ostream& out, const Point& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard fmtguard(out);
  out << '(' << dest.x << ';' << dest.y << ')';
  return out;
}

std::istream& hvostov::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard fmtguard(in);
  size_t size = 0;
  in >> size;
  if (!in || size < 3) {
    in.setstate(std::ios_base::failbit);
    dest.points.clear();
    return in;
  }
  using point = detail::Point;
  std::vector< point > polygon;
  using iit_t = std::istream_iterator< point >;
  std::copy_n(iit_t{in}, size, std::back_inserter(polygon));
  if (in) {
    dest.points = polygon;
  } else {
    in.setstate(std::ios_base::failbit);
    dest.points.clear();
  }
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
  out << size << ' ';
  using oii_t = std::ostream_iterator< detail::Point >;
  std::copy_n(dest.points.begin(), size - 1, oii_t{out, " "});
  out << dest.points[size - 1];
  return out;
}

bool hvostov::isPolygon(const Polygon& p)
{
  return p.points.size() >= 3;
}
