#include <iostream>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <iterator>
#include "geometry.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Please write filepath\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Couldnt open file\n";
    return 1;
  }

  hvostov::data_t data;
  using iit_t = std::istream_iterator< hvostov::Polygon >;
  std::copy(iit_t{file}, iit_t{}, std::back_inserter(data));
  data.erase(std::remove_if(data.begin(), data.end(), hvostov::isPolygonEmpty), data.end());
  std::unordered_map< std::string, hvostov::cmd_t > cmds;
  cmds["AREA"] = hvostov::area;
  cmds["MAX"] = hvostov::max;
  cmds["MIN"] = hvostov::min;
  cmds["COUNT"] = hvostov::count;
  cmds["INFRAME"] = hvostov::inFrame;
  cmds["MAXSEQ"] = hvostov::maxSeq;
  hvostov::process(cmds, data);
}
