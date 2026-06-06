#ifndef INPUT_HPP
#define INPUT_HPP

#include <istream>

namespace hvostov {
  struct Delimiter {
    char exp;
  };
  std::istream& operator>>(std::istream& in, Delimiter&& dest);

  class IOguard {
  public:
    explicit IOguard(std::basic_ios< char >& s);
    ~IOguard();

  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };
}

#endif
