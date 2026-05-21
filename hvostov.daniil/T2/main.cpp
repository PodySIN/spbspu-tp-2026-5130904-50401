#include "types.hpp"
#include <iostream>
#include <istream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <limits>

int main()
{
  using T = hvostov::DataStruct;
  std::vector< T > data;
  using iit_t = std::istream_iterator< T >;
  while (!std::cin.eof()) {
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
    if (std::cin.fail()) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  using oit_t = std::ostream_iterator< T >;
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
}
