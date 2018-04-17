#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
using namespace std;

enum class OutputFormat
{
  Markdown,
  Html
};

struct ListStrategy
{
  virtual ~ListStrategy() = default;
  virtual void add_list_item(ostringstream& oss, const string& item) = 0;
  virtual void start(ostringstream& oss) = 0;
  virtual void end(ostringstream& oss) = 0;
};

struct MarkdownListStrategy : ListStrategy
{
  void start(ostringstream& oss) override
  {
  }

  void end(ostringstream& oss) override
  {
  }

  void add_list_item(ostringstream& oss, const string& item) override
  {
    oss << " * " << item << endl;
  }
};

struct HtmlListStrategy : ListStrategy
{
  void start(ostringstream& oss) override
  {
    oss << "<ul>" << endl;
  }

  void end(ostringstream& oss) override
  {
    oss << "</ul>" << endl;
  }

  void add_list_item(ostringstream& oss, const string& item) override
  {
    oss << "<li>" << item << "</li>" << endl;
  }
};

template <typename LS>
struct TextProcessor
{
  void clear()
  {
    oss.str("");
    oss.clear();
  }
  void append_list(const vector<string> items)
  {
    list_strategy.start(oss);
    for (auto& item : items)
      list_strategy.add_list_item(oss, item);
    list_strategy.end(oss);
  }
  string str() const { return oss.str(); }
private:
  ostringstream oss;
  LS list_strategy;
};

int main()
{
  // markdown
  TextProcessor<MarkdownListStrategy> tpm;
  tpm.append_list({"foo", "bar", "baz"});
  cout << tpm.str() << endl;

  // html
  TextProcessor<HtmlListStrategy> tph;
  tph.append_list({"foo", "bar", "baz"});
  cout << tph.str() << endl;

  getchar();
  return 0;
}
