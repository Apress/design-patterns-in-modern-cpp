#include <string>
#include <vector>
#include <ostream>
using namespace std;

struct Field
{
  string name, type;

  Field(const string& name, const string& type)
    : name{name},
      type{type}
  {
  }


  friend ostream& operator<<(ostream& os, const Field& obj)
  {
    return os << obj.type << " " << obj.name << ";";
  }
};

struct Class
{
  string name;
  vector<Field> fields;

  friend ostream& operator<<(ostream& os, const Class& obj)
  {
    os << "class " << obj.name << "\n{\n";
    for (auto&& field : obj.fields)
    {
      os << "  " << field << "\n";
    }
    return os << "};\n";
  }
};

class CodeBuilder
{
  Class the_class;
public:
  CodeBuilder(const string& class_name)
  {
    the_class.name = class_name;
  }

  CodeBuilder& add_field(const string& name, const string& type)
  {
    the_class.fields.emplace_back(Field{ name, type });
    return *this;
  }

  friend ostream& operator<<(ostream& os, const CodeBuilder& obj)
  {
    return os << obj.the_class;
  }
};

#include "gtest/gtest.h"

//#include "helpers/iohelper.h"

//#include "exercise.cpp"

#include <string>
#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
  ltrim(s);
  return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
  rtrim(s);
  return s;
}

namespace
{
  class Evaluate : public testing::Test
  {
  };

  TEST_F(Evaluate, EmptyTest)
  {
    CodeBuilder cb{ "Foo" };
    ostringstream oss;
    oss << cb;
    auto printed = oss.str();
    trim(printed);
    ASSERT_EQ("class Foo\n{\n};", printed);
  }

  TEST_F(Evaluate, PersonTest)
  {
    auto cb = CodeBuilder{ "Person" }
      .add_field("name", "string")
      .add_field("age", "int");
    ostringstream oss;
    oss << cb;
    auto printed = oss.str();
    trim(printed);
    ASSERT_EQ("class Person\n{\n  string name;\n  int age;\n};", printed);
  }
} // namespace