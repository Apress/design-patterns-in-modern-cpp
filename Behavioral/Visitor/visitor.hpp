#pragma once
#include <string>

struct Paragraph;
struct BoldParagraph;
struct ListItem;
struct List;
struct Div;

struct Visitor
{
	virtual ~Visitor() = default;

	virtual void visit(const Paragraph& p) = 0;
	virtual void visit(const BoldParagraph& p) = 0;
	virtual void visit(const ListItem& li) = 0;
	// here is the problem
	virtual void visit(const List& l) = 0;
	virtual void visit(const Div& div) = 0;

	// need to add another visit function whenever a new type is added

	virtual std::string str() const = 0;
};
