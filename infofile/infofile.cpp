#include "infofile/infofile.h"

#include <cassert>

#include <iostream>

#define DEBUGCPP(x) std::cout << x << " "

namespace infofile {

Value::Value() {
    assert(this);
	DEBUGCPP("value()");
}

Value::Value(const String& value) : value_(value) {
    assert(this);
	DEBUGCPP("value(" << value << ")");
}

Value::~Value() {
    assert(this);
    Clear();
}

const String& Value::value() const{
    assert(this);
    return value_;
}

void Value::set_value(const String& value) {
    assert(this);
    value_ = value;
	DEBUGCPP("set_value(" << value << ")");
}

void Value::Clear() {
    assert(this);

#if !INFOFILE_USE_SHARED_PTR
    for(List::iterator i=children.begin(); i!=children.end(); ++i) {
        delete *i;
    }
    children.clear();
#endif
}

////////////////////////////////////////////////////////

Node::Node(){
	DEBUGCPP("node()");
    assert(this);
}

Node::Node(const String& name) : name_(name) {
	assert(this);
	DEBUGCPP("node(" << name << ")");
}

Node::Node(const String& name, const String& value) : Value(value), name_(name) {
    assert(this);
	DEBUGCPP("node(" << name << ", " << value<< ")");
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
	DEBUGCPP("set_name(" << name << ")");
}

void Node::Clear() {
    assert(this);
    Value::Clear();
    set_name("");
}

}
