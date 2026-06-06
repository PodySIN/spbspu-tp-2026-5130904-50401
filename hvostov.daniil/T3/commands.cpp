#include "commands.hpp"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>
#include <limits>

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
    out << std::count_if(data.begin(), data.end(), std::bind(detail::hasVertexesCount, n, _1)) << "\n";
  }
}

void hvostov::inFrame(std::istream& in, std::ostream& out, const std::vector< Polygon >& data)
{
  using namespace std::placeholders;
  if (data.empty()) {
    out << "<FALSE>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  Polygon test;
  in >> test;

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

  if (target.points.empty() || data.empty()) {
    out << "0\n";
    return;
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
    cmds.at(cmd)(std::cin, std::cout, data);
  } else {
    hvostov::detail::handleError(std::cout, std::cin);
  }
  process(cmds, data);
}
