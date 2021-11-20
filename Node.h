/**
    Justin Hardy
    JEH180008
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <ostream>
#include <sstream>
#include <type_traits>

template<typename T>
class Node {
protected:
    T data;
    Node<T>* left;
    Node<T>* right;
public:
    /// Constructors
    Node() {
        left = nullptr;
        right = nullptr;
    };

    Node(T value) {
        data = value;
        left = nullptr;
        right = nullptr;
    };

    virtual ~Node() {};

    /// Getters
    Node<T>* GetLeft() { return left; };
    Node<T>* GetRight() { return right; };
    T GetData() { return data; };
    std::string ToString() {
        std::stringstream stream;
        stream << data;
        return stream.str();
    };

    /// Setters
    void SetLeft( Node<T>* node ) { left = node; };
    void SetRight( Node<T>* node ) { right = node; };
    void SetData( T value ) { data = value; };

    /// Overrides
    bool operator<( Node<T>* other ) {
        return data < other->data;
    }

    bool operator>( Node<T>* other ) {
        return data > other->data;
    }

    bool operator==( Node<T>* other ) {
        return data == other->data;
    }

    friend std::ostream& operator<<(std::ostream& stream, Node<T>* node) {
        stream << node->data;
        return stream;
    };

    // Allow BinaryTree to access private and protected members of this class.
    template<class C> friend class BinaryTree;
};

#endif // NODE_H
