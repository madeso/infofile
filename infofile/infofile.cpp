#include "infofile/infofile.h"

#include <cassert>

namespace infofile {

Value::Value() {
    assert(this);
}

Value::Value(const String& value) : value_(value) {
    assert(this);
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
}

void Value::Clear() {
    assert(this);
    // todo: delete children
}

////////////////////////////////////////////////////////

Node::Node(){
    assert(this);
}

Node::Node(const String& name) : name_(name) {
    assert(this);
}

Node::Node(const String& name, const String& value) : Value(value), name_(name) {
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

void Node::Clear() {
    assert(this);
    Value::Clear();
    set_name("");
}

}
