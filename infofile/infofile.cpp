#include "infofile/infofile.h"

#include <cassert>

namespace infofile {

namespace {
	int active_node = 0;
	int active_value = 0;
}

Value::Value() {
    assert(this);
	++active_value;
}

Value::~Value() {
    assert(this);
    Clear();
	--active_value;
}

void Value::Clear() {
    assert(this);

    for(List::iterator i=children.begin(); i!=children.end(); ++i) {
        delete *i;
    }
	children.clear();
}

#ifdef _DEBUG
int Value::ActiveCount() {
	return active_value;
}
#endif

////////////////////////////////////////////////////////

Node::Node(){
    assert(this);
	++active_node;
}

Node::Node(const String& name) : name_(name), value_(""), children(0) {
	assert(this);
	++active_node;
}

Node::Node(const String& name, const String& value) : name_(name)
		, value_(value), children(0) {
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

void Node::Clear() {
    assert(this);
	if (children) {
		children->Clear();
	}
    set_name("");
	set_value("");
}

#ifdef _DEBUG
int Node::ActiveCount() {
	return active_node;
}
#endif

}
