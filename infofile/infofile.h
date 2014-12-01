#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <string>
#include <vector>

namespace infofile {

// forward declarations
class Value;
class Node;

typedef Node* NodePtr;

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
    ~Value();

	void Clear();

    List children;

#ifdef _DEBUG
	static int ActiveCount();
#endif

private:
    Value(const Value& other){} // don't allow copying
};

/** A Node in the info file.
 */
class Node {
public:
    Node();
    explicit Node(const String& name);
    Node(const String& name, const String& value);
    ~Node();

    const String& name() const;
    void set_name(const String& name);

	const String& value() const;
    void set_value(const String& value);

	Value* children;

    void Clear();

#ifdef _DEBUG
	static int ActiveCount();
#endif

private:
	String name_;
	String value_;
};

Value* Parse(const String& filename, const String& data, std::vector<std::string>* errors);
Value* ReadFile(const String& filename, std::vector<std::string>* errors);

}

#endif // INFOFILE_INFOFILE_H
