#include <ios>
#include <iostream>
#include <istream>

namespace hvostov {

  struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
  };

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
