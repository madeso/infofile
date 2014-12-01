#ifndef INFOFILE_INFOFILE_H
#define INFOFILE_INFOFILE_H

#include <string>
#include <vector>

#include "configuration.h"

namespace infofile {

// how should strings be handled?
typedef std::string String;

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

	Node* children; // first child
	Node* next; // next child

	Node* GetFirstChild();
	void AddChild(Node* child);
	void SetEndChild(Node* child);
	unsigned int GetSibblingCount();

    void Clear();

#if INFOFILE_USE_BASIC_MEMCHECK
	static int ActiveCount();
#endif

private:
	String name_;
	String value_;
};

Node* Parse(const String& filename, const String& data, std::vector<std::string>* errors);
Node* ReadFile(const String& filename, std::vector<std::string>* errors);

}

#endif // INFOFILE_INFOFILE_H
