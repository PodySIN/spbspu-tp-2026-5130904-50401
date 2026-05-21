#include <ios>
#include <iostream>
#include <istream>

namespace hvostov {

  struct DataStruct {
    double key1;
    unsigned long long key2;
    std::string key3;
  };
  std::istream& operator>>(std::istream& in, DataStruct& dest);
  std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
  bool operator<(const DataStruct& lhs, const DataStruct& rhs);

  struct Delimiter {
    char exp;
  };
  std::istream& operator>>(std::istream& in, Delimiter&& dest);

  struct UllOctI {
    unsigned long long& ref;
  };
  std::istream& operator>>(std::istream& in, UllOctI&& dest);

  struct UllOctO {
    const unsigned long long& ref;
  };
  std::ostream& operator<<(std::ostream& out, const UllOctO& dest);

  struct DoubleSciI {
    double& ref;
  };
  std::istream& operator>>(std::istream& in, DoubleSciI&& dest);

  struct DoubleSciO {
    const double& ref;
  };
  std::ostream& operator<<(std::ostream& out, const DoubleSciO& dest);

  struct StringIO {
    std::string& ref;
  };
  std::istream& operator>>(std::istream& in, StringIO&& dest);

  struct KeyI {
    DataStruct& ref;
  };
  std::istream& operator>>(std::istream& in, KeyI&& key);

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
