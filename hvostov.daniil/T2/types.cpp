#include "types.hpp"
#include <cmath>

std::istream& hvostov::operator>>(std::istream& in, DataStruct& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  DataStruct input;
  using sep = Delimiter;
  using key = KeyI;
  in >> sep{'('};
  in >> sep{':'} >> key{input};
  in >> sep{':'} >> key{input};
  in >> sep{':'} >> key{input};
  in >> sep{':'} >> sep{')'};
  if (in) {
    dest = input;
  }
  return in;
}

std::ostream& hvostov::operator<<(std::ostream& out, const DataStruct& data)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard fmtguard(out);
  out << "(:key1 " << DoubleSciO{data.key1};
  out << ":key2 " << UllOctO{data.key2};
  out << ":key3 \"" << data.key3 << "\":)";
  return out;
}

hvostov::IOguard::IOguard(std::basic_ios< char >& s) :
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{
}

hvostov::IOguard::~IOguard()
{
  s_.width(width_);
  s_.fill(fill_);
  s_.precision(precision_);
  s_.flags(fmt_);
}
