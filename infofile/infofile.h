#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <string>
#include <vector>

#if INFOFILE_USE_SHARED_PTR
#include <memory>
#endif

namespace infofile {

// forward declarations
class Value;
class Node;

#if INFOFILE_USE_SHARED_PTR
typedef std::shared_ptr<Node> NodePtr;
#else
typedef Node* NodePtr;
#endif

// how should strings be handled?
typedef std::string String;

// how should the list and node allocations be handled?
typedef std::vector<NodePtr> List;

/** The value part of the info file.
 * @todo come up with a better name, Root? Tree?
 */
class Value {
public:
    Value();
    explicit Value(const String& value);
    ~Value();

    const String& value() const;
    void set_value(const String& value);

    void Clear();

    List children;
private:
    Value(const Value& other){} // don't allow copying
    String value_;
};

/** A Node in the info file.
 */
class Node : public Value {
public:
    Node();
    explicit Node(const String& name);
    Node(const String& name, const String& value);
    ~Node();

    const String& name() const;
    void set_name(const String& name);

    void Clear();
private:
    String name_;
};

void Parse(const String& data, Value* value, std::vector<std::string>* errors);

}

#endif // INFOFILE_INFOFILE_H
