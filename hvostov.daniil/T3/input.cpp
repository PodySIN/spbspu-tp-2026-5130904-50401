#include "input.hpp"

std::istream& hvostov::operator>>(std::istream& in, Delimiter&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = 0;
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

hvostov::IOguard::IOguard(std::basic_ios< char >& s):
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
