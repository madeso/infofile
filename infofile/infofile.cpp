#include "infofile/infofile.h"

#include <cassert>

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
	if (children) delete children;
	children = 0;
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
	unsigned int count = 0;
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
		children->Clear();
	}
    set_name("");
	set_value("");
}

#if INFOFILE_USE_BASIC_MEMCHECK
int Node::ActiveCount() {
	return active_node;
}
#endif

}
