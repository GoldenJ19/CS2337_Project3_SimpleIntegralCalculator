/**
    Justin Hardy
    JEH180008
*/

#ifndef BINTREE_H
#define BINTREE_H

#include "Node.h"
#include <iostream>
#include <exception>
#include <ostream>
#include <sstream>

template<typename T>
class BinaryTree {
private:
    /// Class Functions
    // Recursive function that adds data to the tree using binary search algorithm.
    Node<T>* Add( T d, Node<T>* n, int h ) {
        if( !n ) {
            // The node does not exist so,
            // If the node is the head,
            if( n == head ) {
                // Set this data as the head node.
                head = new Node<T>(d);
            }
            // Otherwise,
            else {
                // set this node as a new node containing the data desired to be added.
                n = new Node<T>(d);
            }
            // Set height accordingly and increase size.
            height = h;
            size++;
        }
        else if( d < n->data ) {
            // Less than the current data value in the node,
            // so move left.
            n->left = Add(d, n->left, h+1);
        }
        else if( d > n->data ) {
            // Greater than the current data value in the node,
            // so move right.
            n->right = Add(d, n->right, h+1);
        }
        else {
            // Equal to the current data value in the node,
            // so modify the node.
            n->data += d;
        }
        return n;
    };

    // Recursive function that searches for data in the tree, and returns true when the data is found.
    bool Search( T d, Node<T>* n ) {
        if( n ) {
            if( d == n->data ) {
                return true;
            }
            return d < n->data ? Search(d, n->left) : Search(d, n->right);
        }
        return false;
    };

    // Recursive function that gets a Node at a specific index.
    // Indexing is based off of reverse in-order order, and
    // is used solely for the used of a for loop to access items in the tree.
    Node<T>* GetAt( Node<T>* n, int targetIndex, int& currentIndex ) {
        // Each time this loops through, targetNode needs to be set to nullptr.
        Node<T>* targetNode = nullptr;
        // If the current node exists,
        if( n ) {
            // Set the targetNode to the desired index IF its been found.
            // Otherwise, it will just simply be set to nullptr.
            targetNode = GetAt(n->right, targetIndex, currentIndex);

            // Increase the current index by 1.
            currentIndex += 1;
            // If the current index is equal to the target index (desired index is found),
            if(currentIndex == targetIndex) {
                // Return the Node so that the stack is broken down!
                return n;
            }
            // Or if the has already been found (not nullptr),
            else if(targetNode){
                // Return the target node!
                return targetNode;
            }

            // Otherwise, keep searching for the target node.
            targetNode = GetAt(n->left, targetIndex, currentIndex);
        }
        // Return the targetNode, whatever it may be (found or not found).
        return targetNode;
    };

    // Recursive function that gets the parent of the Node of removal in the tree.
    // Used solely by the remove function.
    Node<T>* GetBefore( T data, Node<T>* current ) {
        if( current && current->data != data ) {
            if(data < current->data) {
                if(current->left->data != data) {
                    return GetBefore(data, current->left);
                }
                else {
                    return current;
                }
            }
            else {
                if(current->right->data != data) {
                    return GetBefore(data, current->right);
                }
                else {
                    return current;
                }
            }
        }
        return nullptr;
    };

    // Recursive function that gets the parent of the highest Node in a subtree.
    // Used solely by the remove function.
    Node<T>* GetBeforeHighest( Node<T>* current ) {
        if(current) {
            if( current->right ) {
                if( current->right->right ) {
                    return GetBeforeHighest(current->right);
                }
                return current;
            }
        }
        return current;
    };

    // Recursive function that gets the parent of the lowest Node in a subtree.
    // Used solely by the remove function.
    Node<T>* GetBeforeLowest( Node<T>* current ) {
        if(current) {
            if( current->left ) {
                if( current->left->left ) {
                    return GetBeforeLowest(current->left);
                }
                return current;
            }
        }
        return current;
    };

    // Deletes the binary tree recursively, starting from the leaf up.
    // Utilizes post-order traversal to accomplish this.
    void DeleteBinaryTree(Node<T>* current) {
        if(current) {
            DeleteBinaryTree(current->left);
            DeleteBinaryTree(current->right);
        }
        delete current;
    };

    /// Traversals
    // Recursive function that returns a pre-order representation of the string.
    void GetPreOrder( Node<T>* n, std::stringstream& stream ) {
        if( n ) {
            /// current, left, right.
            stream << n->ToString() << " ";
            GetPreOrder(n->left, stream);
            GetPreOrder(n->right, stream);
        }
        return;
    };

    // Recursive function that returns a in-order representation of the string.
    void GetInOrder( Node<T>* n, std::stringstream& stream ) {
        if( n ) {
            /// left, current, right.
            GetInOrder(n->left, stream);
            stream << n->ToString() << " ";
            GetInOrder(n->right, stream);
        }
        return;
    };

    // Recursive function that returns a reverse in-order representation of the string.
    void GetReverseInOrder( Node<T>* n, std::stringstream& stream ) {
        if( n ) {
            /// right, current, left.
            GetReverseInOrder(n->right, stream);
            stream << n->ToString() << " ";
            GetReverseInOrder(n->left, stream);
        }
        return;
    };

    // Recursive function that returns a post-order representation of the string.
    void GetPostOrder( Node<T>* n, std::stringstream& stream ) {
        if( n ) {
            /// left, right, current.
            GetPostOrder(n->left, stream);
            GetPostOrder(n->right, stream);
            stream << n->ToString() << " ";
        }
        return;
    };

    // Recursive function that concatenates a string to represent the tree in an easy-to-read format.
    void PrintTree( Node<T>* n, int time, int previousLength, std::stringstream& stream ) {
        /*
            n:              Node that holds the current node that the recursive loop is at.
            time:           Helps keep track of how deep into the tree we are. Better referred to as "depth".
            previousLength: Keeps track of the previous length of the parent Node's string.
            stream:         Help concatenate the string to avoid compiler issues.
        */
        if( n ) {
            std::string spaces = "";
            for(int i = 0; i < time; i++) {
                spaces += "  ";
            }
            for(int i = 0; i < previousLength-1; i++) {
                spaces += " ";
            }
            std::string dataString = n->ToString();
            int lengthOfDataString = dataString.size(); // Get the length of the current data's string.

            // Use reverse in-order traversal to recursively loop through the tree.
            PrintTree(n->right, time+1, lengthOfDataString+previousLength, stream);
            stream << spaces << dataString << std::endl;
            PrintTree(n->left, time+1, lengthOfDataString+previousLength, stream);
        }
        return;
    };
protected:
    int size, height;
    Node<T>* head;
public:
    /// Constructors
    BinaryTree() {
        size = 0;
        height = 0;
        head = nullptr;
    };

    virtual ~BinaryTree() {
        DeleteBinaryTree(head);
    };

    /// Class Functions
    // Adds a data member to the tree.
    bool Add( T d ) {
        try {
            Add(d, head, 0);
            return true;
        }
        catch(std::exception e) {
            std::cout << "ERROR! Failed to add to Binary Tree." << std::endl;
            return false;
        }
    };

    // Removes a specific data member from the tree, and readjusts the tree if necessary.
    T Remove( T d ) {
        // If the head exists,
        if(head) {
            // Get the Node before the Node of deletion.
            Node<T>* beforeReplace = GetBefore(d, head);

            // Find out of the Node of deletion is beforeCurrent or actually a child of beforeCurrent.
            Node<T>* replace = head;
            short direction = 0;
            if(beforeReplace) { // beforeReplace will be nullptr if the node of deletion is the head.
                // Get the direction of the Node we want to delete in
                // accordance to beforeCurrent and get the Node of deletion.
                direction = d < beforeReplace->data ? -1 : 1;
                replace = direction == -1 ? beforeReplace->left : beforeReplace->right;
            }

            if(replace->left) {
                // Find highest left-side node to replace current node's data.
                Node<T>* beforeHighest = GetBeforeHighest(replace->left);
                if(beforeHighest->right) {
                    // Get the highest of the subtree to the left of the Node of deletion.
                    Node<T>* highest = beforeHighest->right;
                    // Move data from highest to replace and delete highest.
                    replace->data = highest->data;
                    // Move over highest's left subtree to beforeHighest's right pointer.
                    beforeHighest->right = highest->left;
                    delete highest;
                }
                else {
                    // Move data from beforeHighest to replace.
                    replace->data = beforeHighest->data;
                    replace->left = beforeHighest->left;
                }
            }
            else if(replace->right) {
                // Find lowest right-side node to replace current node's data.
                Node<T>* beforeLowest = GetBeforeLowest(replace->right);
                if(beforeLowest->left) {
                    // Get the lowest of the subtree to the right of the Node of deletion.
                    Node<T>* lowest = beforeLowest->left;
                    // Move data from lowest to replace and delete lowest.
                    replace->data = lowest->data;
                    // Move over lowest's left subtree to beforeLowest's right pointer.
                    beforeLowest->left = lowest->right;
                    delete lowest;
                }
                else {
                    // Move data from beforeLowest to replace.
                    replace->data = beforeLowest->data;
                    replace->right = beforeLowest->right;
                }
            }
            else {
                // Delete this node.
                if (direction == 0) {
                    // The deleted node is the head.
                    delete head;
                    head = nullptr;
                }
                else if(direction == -1) {
                    // Delete this node from the tree.
                    beforeReplace->left = nullptr;
                    delete replace;
                }
                else if(direction == 1){
                    // Delete this node from the tree.
                    beforeReplace->right = nullptr;
                    delete replace;
                }
                else {
                    // Direction is equal to an invalid number (=/= 0, +-1)
                    std::cout << "ERROR. Invalid Direction.";
                }
            }
            size--;
            return d;
        }
    };

    // Searches the tree for a specific value.
    // Returns true if the value exists in the tree, and false if it does not.
    bool Search( T d ) {
        try {
            return Search(d, head);
        }
        catch(std::exception e) {
            std::cout << "ERROR! Failed to perform a search in Binary Tree." << std::endl;
            return false;
        }
    };

    // Indexing based off of Reverse In-Order Traversal Order
    T& GetAt( int index ) {
        Node<T>* targetNode = nullptr;
        int currentIndex = -1;
        targetNode = GetAt(head, index, currentIndex);
        return targetNode->data;
    };

    /// Traversals
    std::string GetPreOrder() {
        std::stringstream stream;
        GetPreOrder(head, stream);
        return stream.str();
    };

    std::string GetInOrder() {
        std::stringstream stream;
        GetInOrder(head, stream);
        return stream.str();
    };

    std::string GetReverseInOrder() {
        std::stringstream stream;
        GetReverseInOrder(head, stream);
        return stream.str();
    };

    std::string GetPostOrder() {
        std::stringstream stream;
        GetPostOrder(head, stream);
        return stream.str();
    };

    std::string PrintTree() {
        std::stringstream stream;
        PrintTree(head, 0, 0, stream);
        return stream.str();
    };

    /// Getters
    // Returns the head's data.
    T GetHead() {
        return head->data;
    };

    // Returns the size of the tree.
    int Size() {
        return size;
    };

    // Returns the height of the tree.
    int Height() {
        return height;
    };

    /// Setters
    // Deletes the head node, clearing the tree.
    bool Clear(){
        try {
            DeleteBinaryTree();
            head = nullptr;
            return true;
        }
        catch (std::exception& e) {
            std::cout << "ERROR! Failed to clear BinaryTree." << std::endl;
            return false;
        }
    };

    /// Overloads
    // Prints the binary tree in a formatted and easy-to-read output.
    friend std::ostream& operator<<( std::ostream& stream, BinaryTree& tree ) {
        stream << tree.PrintTree();
        return stream;
    };
};

#endif // BINTREE_H
