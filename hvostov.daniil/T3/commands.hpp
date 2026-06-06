#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <iosfwd>
#include <vector>
#include <functional>
#include "geometry.hpp"
#include <unordered_map>
#include <vector>

namespace hvostov {

  void area(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void max(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void min(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void count(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void inFrame(std::istream&, std::ostream&, const std::vector< Polygon >&);
  void maxSeq(std::istream&, std::ostream&, const std::vector< Polygon >&);

  using cmd_t = std::function< void(std::istream&, std::ostream&, const std::vector< hvostov::Polygon >&) >;
  using data_t = std::vector< hvostov::Polygon >;
  void process(std::unordered_map< std::string, cmd_t >& cmds, data_t data);
}

#endif
