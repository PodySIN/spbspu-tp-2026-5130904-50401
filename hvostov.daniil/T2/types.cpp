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

bool hvostov::operator<(const DataStruct& lhs, const DataStruct& rhs)
{
  return (lhs.key1 < rhs.key1) || (lhs.key1 == rhs.key1 && lhs.key2 < rhs.key2) ||
         (lhs.key1 == rhs.key1 && lhs.key2 == rhs.key2 && lhs.key3 < rhs.key3);
}

std::istream& hvostov::operator>>(std::istream& in, KeyI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string k;
  in >> k;
  if (k == "key1") {
    using dblsci = DoubleSciI;
    in >> dblsci{dest.ref.key1};
  } else if (k == "key2") {
    using ulloct = UllOctI;
    in >> ulloct{dest.ref.key2};
  } else if (k == "key3") {
    using str = StringIO;
    in >> str{dest.ref.key3};
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

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

std::istream& hvostov::operator>>(std::istream& in, StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> Delimiter{'"'}, dest.ref, '"');
}

std::istream& hvostov::operator>>(std::istream& in, DoubleSciI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> dest.ref;
}

std::ostream& hvostov::operator<<(std::ostream& out, const DoubleSciO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard fmtguard(out);
  double value = dest.ref;
  if (value == 0.0) {
    out << "0.0e+0";
  } else {
    int exponent = 0;
    double mantissa = value;
    if (mantissa > 0) {
      while (mantissa >= 10.0) {
        mantissa /= 10.0;
        exponent++;
      }
      while (mantissa < 1.0) {
        mantissa *= 10.0;
        exponent--;
      }
    } else if (mantissa < 0) {
      while (mantissa <= -10.0) {
        mantissa /= 10.0;
        exponent++;
      }
      while (mantissa > -1.0) {
        mantissa *= 10.0;
        exponent--;
      }
    }
    mantissa = std::round(mantissa * 10.0) / 10.0;
    out << mantissa << "e";
    if (exponent >= 0) {
      out << "+";
    }
    out << exponent;
  }
  return out;
}

std::istream& hvostov::operator>>(std::istream& in, UllOctI&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  IOguard fmtguard(in);
  in.unsetf(std::ios_base::basefield);
  in >> dest.ref;
  return in;
}

std::ostream& hvostov::operator<<(std::ostream& out, const UllOctO& dest)
{
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  IOguard fmtguard(out);
  out << "0" << std::oct << dest.ref;
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
