#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <string>
#include <vector>

namespace infofile {

// forward declarations
class Value;
class Node;

// how should strings be handled?
typedef std::string String;

// how should the list and node allocations be handled?
typedef std::vector<Node*> List;

class Value {
public:
    Value();
    explicit Value(const String& value);
    ~Value();

    const String& value() const;
    void set_value(const String& value);

    List children;
private:
    String value_;
};

class Node : public Value {
public:
    Node();
    explicit Node(const String& name);
    Node(const String& name, const String& value);
    ~Node();

    const String& name() const;
    void set_name(const String& name);
private:
    String name_;
};

}

#endif // INFOFILE_INFOFILE_H
