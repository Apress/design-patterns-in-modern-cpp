#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <memory>
using namespace std;

#include "visitor.hpp"
#include "model.hpp"

struct HtmlVisitor : Visitor
{
	void visit(const Paragraph& p) override
	{
		oss << "<p>" << p.text << "</p>\n";
	}

	void visit(const ListItem& li) override
	{
		oss << "<li>" << li.text << "</li>\n";
	}

	void visit(const List& l) override
	{
		oss << "<ul>\n";
		for_each(l.begin(), l.end(), [&](const ListItem& li)
		{
			// only acceptable if we know that li is a ListItem
			this->visit(li);

			// if li was an Element, we would do
			//li.accept(*this);
		});
		oss << "</ul>\n";
	}

	string str() const override
	{
		return oss.str();
	}

private:
	ostringstream oss;
};

struct MarkdownVisitor : Visitor
{
	// dynamic_cast ifs
	void visit(const Element& e)
	{
		if (const Paragraph* p = dynamic_cast<const Paragraph*>(&e))
		{
			visit(*p);
		} else
		{
			// other cases
		}

	}

	void visit(const Paragraph& p) override
	{
		oss << p.text << "\n";
	}
	
	void visit(const BoldParagraph& p) override
	{
		oss << "*" << p.text << "*\n";
	}

	void visit(const ListItem& li) override
	{
		oss << " * " << li.text << "\n";
	}

	void visit(const List& l) override
	{
		oss << "\n";
		for_each(l.begin(), l.end(), [&](const ListItem& li) { this->visit(li); });
		oss << "\n";
	}


	void visit(const Div& div) override
	{
		oss << "<div>\n";
		for (auto e : div)
		{
			//visit(*e);
			e->accept(*this);
		}
		oss << "</div>\n";
	}

	string str() const override
	{
		return oss.str();
	}

private:
	ostringstream oss;
};

// adding a new class requires total recompilation

int main_(int argc, char* argv[])
{
	BoldParagraph p{ "Here are some colors" };
	ListItem red{ "Red" };
	ListItem green{ "Green" };
	ListItem blue{ "Blue" };
	List colors{ red,green,blue };

	// single dispatch needs 1 item of information: which element to render to text
	// double dispatch needs 2 pieces of information: which element to render, and using which renderer

	vector<Element*> document{ &p, &colors };
	cout << "Textual representation: " << endl;

	MarkdownVisitor v;
	ostringstream oss;
	//for_each(document.begin(), document.end(), [&](const Element* e)
	//{
	//	// v.visit(*e) won't work :), but...
	//	e->accept(v);
	//});
	Div div{ &p, &colors };
	div.print_html(oss);
	cout << oss.str() << endl;

	// everything is fine until we want another representation

	getchar();
	return 0;
}
