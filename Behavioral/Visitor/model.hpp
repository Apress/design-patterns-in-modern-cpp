#pragma once
#include <sstream>

struct Visitor;

struct Element
{
	virtual ~Element() = default;

	// print
	virtual void print_html(ostringstream& oss) const = 0;
	// violates open-closed
	// virtual void print_markdown(ostringstream& oss) const = 0;

	// visitor
	virtual void accept(Visitor& v) const = 0;
};

struct Div : vector<Element*>, Element
{
	Div(const initializer_list<value_type>& _Ilist)
		: vector<Element*>(_Ilist)
	{
	}


	void accept(Visitor& v) const override
	{
	}

	void print_html(ostringstream& oss) const override
	{
		oss << "<div>\n";
		for_each(begin(), end(), [&](const Element* e)
		{
			e->print_html(oss);
		});
		oss << "</div>\n";
	}
};

struct TextElement : Element
{
	explicit TextElement(const string& text)
		: text(text)
	{
	}

	string text;
};

struct Paragraph : TextElement
{
	explicit Paragraph(const string& text)
		: TextElement(text)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}

	void print_html(ostringstream& oss) const override
	{
		oss << "<p>" << text << "</p>\n";
	}
};

struct BoldParagraph : Paragraph
{
	explicit BoldParagraph(const string& text)
		: Paragraph(text)
	{
	}


	void accept(Visitor& v) const override {
		v.visit(*this);
	}
};

struct ListItem : TextElement
{
	explicit ListItem(const string& text)
		: TextElement(text)
	{
	}

	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}

	void print_html(ostringstream& oss) const override
	{
		oss << "<li>" << text << "</li>\n";
	}
};

struct List : Element, vector<ListItem>
{
	List(const initializer_list<value_type>& list)
		: vector<ListItem>(list)
	{
	}


	void accept(Visitor& v) const override
	{
		v.visit(*this);
	}

	void print_html(ostringstream& oss) const override
	{
		oss << "<ul>\n";
		for_each(begin(), end(), [&](const ListItem& li) { li.print_html(oss); });
		oss << "</ul>\n";
	}
};