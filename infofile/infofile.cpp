#include "infofile/infofile.h"

#include <cassert>
#include <sstream>
#include <iostream>

namespace infofile {

#if INFOFILE_USE_BASIC_MEMCHECK
	namespace {
		int active_node = 0;
	}
#endif

	////////////////////////////////////////////////////////

	Node::Node() : children(NULL), next(NULL) {
		assert(this);
		++active_node;
	}

	Node::Node(const String& name) : name_(name), value_(""), children(NULL), next(NULL) {
		assert(this);
		++active_node;
	}

	Node::Node(const String& name, const String& value) : name_(name)
		, value_(value), children(NULL), next(NULL) {
		assert(this);
		++active_node;
	}

	Node::~Node(){
		assert(this);
		--active_node;
		Clear();
	}

	const String& Node::name() const{
		assert(this);
		return name_;
	}

	void Node::set_name(const String& name) {
		assert(this);
		name_ = name;
	}

	const String& Node::value() const{
		assert(this);
		return value_;
	}

	void Node::set_value(const String& value) {
		assert(this);
		value_ = value;
	}

	Node* Node::GetFirstChild() {
		assert(this);
		if (children == NULL) return NULL;
		return children;
	}

	void Node::AddChild(Node* child) {
		assert(this);
		assert(child);
		if (children == NULL) children = child;
		else children->SetEndChild(child);
	}

	void Node::SetEndChild(Node* child) {
		assert(this);
		assert(child);
		Node* self = this;
		Node* n = self->next;
		while (true) {
			if (n == NULL) {
				self->next = child;
				return;
			}
			self = n;
			n = self->next;
		}
	}

	unsigned int Node::GetSibblingCount() {
		unsigned int count = 1; // start at 1 sincew we include this in the calculation
		Node* n = next;
		while (n) {
			++count;
			n = n->next;
		}

		return count;
	}

	void Node::Clear() {
		assert(this);
		if (children) {
			delete children;
			children = NULL;
		}
		while (next) {
			Node* t = next->next;;
			delete next;
			next = t;
		}
		next = NULL;
		set_name("");
		set_value("");
	}

#if INFOFILE_USE_BASIC_MEMCHECK
	int Node::ActiveCount() {
		return active_node;
	}
#endif

	Printer::~Printer() {}

	PrintOptions::PrintOptions() : tab("\t"), newline("\n"), term(";") {
	}

	void PrintString(std::stringstream& ss, const String& str) {
		String::size_type index = str.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
		if (index == -1) {
			ss << str;
		}
		else {
			ss << "\"" << str << "\"";
		}
	}

	void PrintNode(Printer* printer, int indent, const PrintOptions& po, Node* node) {
		String tab;
		for (int i = 0; i < indent; ++i) {
			tab += po.tab;
		}
		std::stringstream ss;
		ss << tab; PrintString(ss, node->name()); ss << " "; PrintString(ss, node->value());
		if (node->children) {
			ss << " {" << po.newline;
			printer->Print(ss.str());

			for (Node* c = node->children; c; c = c->next) {
				PrintNode(printer, indent + 1, po, c);
			}

			ss.str("");
			ss << tab << "}";
		}

		ss << po.term << po.newline;
		printer->Print(ss.str());
	}

	void Print(Printer* printer, const PrintOptions& po, Node* node) {
		for (Node* c = node; c; c = c->next) {
			PrintNode(printer, 0, po, c);
		}
	}

	class StdStringStreamPrinter : public Printer {
	public:
		std::stringstream ss;
		virtual void Print(const String& str) {
			ss << str;
		}
	};
	std::string PrintToString(const PrintOptions& po, Node* node) {
		StdStringStreamPrinter ss;
		Print(&ss, po, node);
		return ss.ss.str();
	}

	class CoutPrinter : public Printer {
	public:
		virtual void Print(const String& str) {
			std::cout << str;
		}
	};

	void PrintToConsole(const PrintOptions& po, Node* node) {
		CoutPrinter ss;
		Print(&ss, po, node);
	}
}
