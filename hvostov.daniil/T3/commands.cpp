#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <ios>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <limits>

double hvostov::detail::triangleArea(const std::vector< Point >& pts, size_t i)
{
  const auto& a = pts[0];
  const auto& b = pts[i];
  const auto& c = pts[i + 1];
  double cross = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
  return cross * 0.5;
}

double hvostov::detail::getArea(const Polygon& p)
{
  using namespace std::placeholders;
  if (p.points.size() < 3) {
    return 0.0;
  }

  std::vector< size_t > indices(p.points.size() - 2);
  std::iota(indices.begin(), indices.end(), 1);

  std::vector< double > areas(indices.size());
  std::transform(indices.begin(), indices.end(), areas.begin(), std::bind(triangleArea, std::cref(p.points), _1));

  double sum = std::accumulate(areas.begin(), areas.end(), 0);
  return std::abs(sum);
}

bool hvostov::detail::isEvenVertexes(const Polygon& p)
{
  return p.points.size() % 2 == 0;
}

bool hvostov::detail::isOddVertexes(const Polygon& p)
{
  return p.points.size() % 2 == 1;
}

bool hvostov::detail::hasVertexesCount(size_t n, const Polygon& p)
{
  return p.points.size() == n;
}

size_t hvostov::detail::getVertexesCount(const Polygon& p)
{
  return p.points.size();
}

int hvostov::detail::getX(const Point& p)
{
  return p.x;
}

int hvostov::detail::getY(const Point& p)
{
  return p.y;
}

bool hvostov::detail::isPointInBoundingBox(const Point& p, int minX, int minY, int maxX, int maxY)
{
  return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
}

bool hvostov::detail::pointEqual(const Point& p1, const Point& p2)
{
  return p1.x == p2.x && p1.y == p2.y;
}

bool hvostov::detail::polygonsEqual(const Polygon& a, const Polygon& b)
{
  using namespace std::placeholders;
  if (a.points.size() != b.points.size())
    return false;
  return std::equal(a.points.begin(), a.points.end(), b.points.begin(), std::bind(pointEqual, _1, _2));
}

hvostov::detail::SeqState hvostov::detail::updateSeqState(SeqState state, bool match)
{
  if (match) {
    state.current++;
    if (state.current > state.max)
      state.max = state.current;
  } else {
    state.current = 0;
  }
  return state;
}

double hvostov::detail::sumAreasOfFiltered(const std::vector< Polygon >& data,
                                           std::function< bool(const Polygon&) > pred)
{
  std::vector< Polygon > filtered(data.size());
  auto it = std::copy_if(data.begin(), data.end(), filtered.begin(), pred);
  filtered.erase(it, filtered.end());

  std::vector< double > areas(filtered.size());
  std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);

  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

int hvostov::detail::getPolygonMinX(const Polygon& p)
{
  using namespace std::placeholders;
  auto it = std::min_element(p.points.begin(), p.points.end(),
                             std::bind(std::less< int >(), std::bind(getX, _1), std::bind(getX, _2)));
  return it->x;
}

int hvostov::detail::getPolygonMaxX(const Polygon& p)
{
  using namespace std::placeholders;
  auto it = std::max_element(p.points.begin(), p.points.end(),
                             std::bind(std::less< int >(), std::bind(getX, _1), std::bind(getX, _2)));
  return it->x;
}

int hvostov::detail::getPolygonMinY(const Polygon& p)
{
  using namespace std::placeholders;
  auto it = std::min_element(p.points.begin(), p.points.end(),
                             std::bind(std::less< int >(), std::bind(getY, _1), std::bind(getY, _2)));
  return it->y;
}

int hvostov::detail::getPolygonMaxY(const Polygon& p)
{
  using namespace std::placeholders;
  auto it = std::max_element(p.points.begin(), p.points.end(),
                             std::bind(std::less< int >(), std::bind(getY, _1), std::bind(getY, _2)));
  return it->y;
}

void hvostov::area(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  std::string param;
  in >> param;

  if (param == "MEAN" && data.empty()) {
    throw std::logic_error("No polygons");
  }
  out << std::fixed << std::setprecision(1);

  if (param == "MEAN") {
    std::vector< double > areas(data.size());
    std::transform(data.begin(), data.end(), areas.begin(), detail::getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum / data.size() << "\n";
  } else if (param == "EVEN") {
    out << detail::sumAreasOfFiltered(data, detail::isEvenVertexes) << "\n";
  } else if (param == "ODD") {
    out << detail::sumAreasOfFiltered(data, detail::isOddVertexes) << "\n";
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::logic_error("Invalid polygon");
    }
    out << detail::sumAreasOfFiltered(data, std::bind(detail::hasVertexesCount, n, _1)) << "\n";
  }
}

void hvostov::max(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  std::string param;
  in >> param;

  if (data.empty()) {
    throw std::logic_error("No polygons");
  }

  out << std::fixed << std::setprecision(1);

  if (param == "AREA") {
    std::vector< double > areas(data.size());
    std::transform(data.begin(), data.end(), areas.begin(), detail::getArea);
    auto maxIt = std::max_element(areas.begin(), areas.end());
    out << *maxIt << "\n";
  } else if (param == "VERTEXES") {
    std::vector< size_t > vertexes(data.size());
    std::transform(data.begin(), data.end(), vertexes.begin(), detail::getVertexesCount);
    auto maxIt = std::max_element(vertexes.begin(), vertexes.end());
    out << *maxIt << "\n";
  } else {
    throw std::logic_error("Invalid parameter");
  }
}

void hvostov::min(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  std::string param;
  in >> param;

  if (data.empty()) {
    throw std::logic_error("No polygons");
  }

  out << std::fixed << std::setprecision(1);

  if (param == "AREA") {
    std::vector< double > areas(data.size());
    std::transform(data.begin(), data.end(), areas.begin(), detail::getArea);
    auto minIt = std::min_element(areas.begin(), areas.end());
    out << *minIt << "\n";
  } else if (param == "VERTEXES") {
    std::vector< size_t > vertexes(data.size());
    std::transform(data.begin(), data.end(), vertexes.begin(), detail::getVertexesCount);
    auto minIt = std::min_element(vertexes.begin(), vertexes.end());
    out << *minIt << "\n";
  } else {
    throw std::logic_error("Invalid parameter");
  }
}

void hvostov::count(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  std::string param;
  in >> param;

  if (param == "EVEN") {
    out << std::count_if(data.begin(), data.end(), detail::isEvenVertexes) << "\n";
  } else if (param == "ODD") {
    out << std::count_if(data.begin(), data.end(), detail::isOddVertexes) << "\n";
  } else {
    size_t n = std::stoul(param);
    if (n < 3) {
      throw std::logic_error("Invalid polygon");
    }
    out << std::count_if(data.begin(), data.end(), std::bind(detail::hasVertexesCount, n, _1)) << "\n";
  }
}

void hvostov::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  Polygon test;
  in >> test;

  if (!in || test.points.size() < 3) {
    throw std::logic_error("Invalid polygon");
  }

  if (data.empty()) {
    out << "<FALSE>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  auto polyMinX = std::min_element(
      data.begin(), data.end(),
      std::bind(std::less< int >(), std::bind(detail::getPolygonMinX, _1), std::bind(detail::getPolygonMinX, _2)));
  int minX = detail::getPolygonMinX(*polyMinX);

  auto polyMaxX = std::max_element(
      data.begin(), data.end(),
      std::bind(std::less< int >(), std::bind(detail::getPolygonMaxX, _1), std::bind(detail::getPolygonMaxX, _2)));
  int maxX = detail::getPolygonMaxX(*polyMaxX);

  auto polyMinY = std::min_element(
      data.begin(), data.end(),
      std::bind(std::less< int >(), std::bind(detail::getPolygonMinY, _1), std::bind(detail::getPolygonMinY, _2)));
  int minY = detail::getPolygonMinY(*polyMinY);

  auto polyMaxY = std::max_element(
      data.begin(), data.end(),
      std::bind(std::less< int >(), std::bind(detail::getPolygonMaxY, _1), std::bind(detail::getPolygonMaxY, _2)));
  int maxY = detail::getPolygonMaxY(*polyMaxY);

  std::vector< bool > results(test.points.size());
  std::transform(test.points.begin(), test.points.end(), results.begin(),
                 std::bind(detail::isPointInBoundingBox, _1, minX, minY, maxX, maxY));

  bool allInside = std::all_of(results.begin(), results.end(), std::bind(std::logical_and< bool >(), _1, _1));

  out << (allInside ? "<TRUE>" : "<FALSE>") << "\n";
}

void hvostov::maxSeq(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  Polygon target;
  in >> target;

  if (!in || target.points.size() < 3) {
    throw std::logic_error("Invalid polygon");
  }

  if (target.points.empty() || data.empty()) {
    throw std::logic_error("Invalid polygon");
  }

  std::vector< bool > matches(data.size());
  std::transform(data.begin(), data.end(), matches.begin(), std::bind(detail::polygonsEqual, _1, std::cref(target)));

  detail::SeqState init{0, 0};
  detail::SeqState result =
      std::accumulate(matches.begin(), matches.end(), init, std::bind(detail::updateSeqState, _1, _2));

  out << result.max << "\n";
}

void hvostov::detail::handleError(std::ostream& out, std::istream& in)
{
  out << "<INVALID COMMAND>\n";
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

void hvostov::process(std::unordered_map< std::string, cmd_t >& cmds, data_t data)
{
  std::string cmd;
  if (!(std::cin >> cmd)) {
    return;
  }
  if (cmds.find(cmd) != cmds.end()) {
    try {

      cmds.at(cmd)(std::cin, std::cout, data);
    } catch (...) {
      hvostov::detail::handleError(std::cout, std::cin);
    }
  } else {
    hvostov::detail::handleError(std::cout, std::cin);
  }
  process(cmds, data);
}
