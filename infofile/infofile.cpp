#include "infofile/infofile.h"

#include <cassert>

#include <iostream>

namespace infofile {

Value::Value() {
    assert(this);
}

Value::~Value() {
    assert(this);
    Clear();
}

void Value::Clear() {
    assert(this);

#if !INFOFILE_USE_SHARED_PTR
    for(List::iterator i=children.begin(); i!=children.end(); ++i) {
        delete *i;
    }
#endif
	children.clear();
}

////////////////////////////////////////////////////////

Node::Node(){
    assert(this);
}

Node::Node(const String& name) : name_(name), value_(""), children(0) {
	assert(this);
}

Node::Node(const String& name, const String& value) : name_(name), value_(value), children(0) {
    assert(this);
}

Node::~Node(){
    assert(this);
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

}
