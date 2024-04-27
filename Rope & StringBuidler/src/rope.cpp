#include "rope.hpp"

/*
* Rope class implementation
* Copyright (C) 2022 dhruv
*/

/**
 * Constructor for the Rope class. Initializes a new Rope object with a given string and length.
 *
 * @param str The string to initialize the Rope with.
 * @param len The length of the string to initialize the Rope with.
 *
 * @return None.
 *
 * @throws None.
 */
Rope::Rope(const char str[], uint32_t len) : root(nullptr)
{
    root = new Node(str, len);
}

/**
 * Constructor for the Rope class. Reads a file and creates a new Rope object with the contents of the file.
 *
 * @param filename The name of the file to read.
 *
 * @throws std::runtime_error if the file cannot be opened or read.
 */
Rope::Rope(const char filename[])
{
    // TODO: Read file and create rope
}

Rope::Rope() : root(nullptr) {}

/**
 * Destructor for the Rope class. Deletes the root node of the rope.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
Rope::~Rope()
{
    delete root;
}

/*
* ROPE MANUPULATION FUNCTIONS
* ===========================
* The following functions are used to manipulate the rope data structure. 
* They are used to perform various operations on the rope data structure.
* The functions are used in the Rope class.
* The functions are defined below:
* - merge
* - split
*/

/**
 * Merges two nodes into a new node and returns it. If either of the nodes is null, the non-null node is returned.
 * Updates the weight and height of the new node and performs rebalancing if the height difference between the subtrees exceeds a certain threshold.
 *
 * @param left The left node to be merged.
 * @param right The right node to be merged.
 *
 * @return A new node that is the result of merging the left and right nodes.
 *
 * @throws None.
 */
Rope::Node* Rope::merge(Node* left, Node* right)
{   
    if (left == nullptr)    {return right;}

    if (right == nullptr)   {return left;}
    cout << "Merge" << endl;
    cout << "Merging : " << left->toString() << " and " << right->toString() << endl;
    Node* newNode = new Node(left, right);
    cout << "New Node created : " << newNode->toString() << endl;

    newNode->updateWeight();
    newNode->updateHeight();

    cout << "Update done : " << newNode->toString() << endl;
    cout << "Height difference : " << abs(int(left->getHeight() - right->getHeight())) << endl;
    if (abs(int(left->getHeight() - right->getHeight())) > 1) {
        //rebalancing - Balancing is only performed when the height difference between subtrees exceeds a certain threshold, reducing unnecessary balancing operations.
        cout << "Need to Rebalance" << endl;
        newNode = rebalance(newNode);
    }

    cout << "Merged : " << newNode->toString() << endl;
    return newNode;
}

/**
 * Splits a given node at a specified position.
 *
 * @param node The node to be split.
 * @param pos The position at which the split should occur.
 *
 * @return A pair of nodes where the first node contains the elements before the split position and the second node contains the elements after the split position. If the split position is 0, the first node is null and the second node contains the entire original node. If the split position is greater than or equal to the length of the node, the first node contains the entire original node and the second node is null.
 *
 * @throws None.
 */
std::pair<Rope::Node*, Rope::Node*> Rope::split(Node* node, uint32_t pos)
{
    if (node == nullptr) {
        return {nullptr, nullptr};
    }
    
    if (node->getIsLeaf()) {
        cout << "Splitting A LEAF: " << node->toString() << " at " << pos << " w/ length " << node->getLength() << endl;
        uint32_t len = node->getLength();
        if (pos == 0) {
            return {nullptr, node};
        }
        else if (pos >= len) {
            return {node, nullptr};
        }
        else {
            char* data = node->getData();
            Node* left = new Node(data, pos);
            Node* right = new Node(data + pos, len - pos);
            cout << "Split done : " << left->toString() << " and " << right->toString() << endl;
            return {left, right};
        }
    }
    else {
        cout << "Splitting A NODE: " << node->toString() << " at " << pos << " w/ weight " << node->getWeight() << endl;
        uint32_t leftWeight = node->getLeft()->getWeight();
        cout << "leftWeight : " << leftWeight << endl;
        if (pos <= leftWeight) {
            cout << "goning left and spliting into " << node->getLeft()->toString() << " at " << pos << " " << endl;
            auto [left, right] = split(node->getLeft(), pos);
            cout << "left split done : " << left->toString() << " and " << right->toString() << endl;
            cout << "returning : " << left ->toString() << " and merged (" << right->toString() << " and " << node->getRight()->toString() << ")" << endl;
            return {left, merge(right, node->getRight())};
        }
        else {
            cout << "going right and spliting into " << node->getRight()->toString() << " at " << pos - leftWeight << " " << endl;
            auto [left, right] = split(node->getRight(), pos - leftWeight);
            cout << "right split done : " << left->toString() << " and " << right->toString() << endl;
            cout << "returning : " << node->getLeft()->toString() << " and merged (" << left->toString() << " and " << right->toString() << ")" << endl;
            return {merge(node->getLeft(), left), right};
        }
    }
}

/*
* ROPE BALANCING FUNCTIONS
* ======================== 
* These functions are used to perform balancing operations on the Rope data structure.
* - rotateLeft
* - rotateRight
* - rebalance
*/

/**
 * Rebalances the given node in the Rope data structure.
 *
 * @param node The node to be rebalanced.
 *
 * @return The rebalanced node.
 *
 * @throws None.
 */
Rope::Node* Rope::rebalance(Node* node)
{
    if (node == nullptr || node->getIsLeaf()) return node;
    if (node->getLeft() == nullptr || node->getRight() == nullptr) return node;
    if (node->getLeft()->getIsLeaf() && node->getRight()->getIsLeaf()) return node;

    int balance = node->balanceFactor();
    if (balance <= 1 && balance >= -1) return node; // Lazy-Balancing - No need to rebalance everytime 

    Node *left = node->getLeft();
    Node *right = node->getRight();
    cout << "Balance Factor : " << balance << endl;
    if (balance > 1) { // Left heavy
        if (left->balanceFactor() < 0)
            node->setLeft(rotateLeft(left));

        cout << "[1]Left heavy" <<  "Node : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;
        node = rotateRight(node);
        cout << "[2]Right heavy" <<  "Node : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;
    }
    else if (balance < -1) { // Right heavy
        if (right->balanceFactor() > 0)
            node->setRight(rotateRight(right));

        cout << "[1]Right heavy" <<  "Node : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;
        node = rotateLeft(node);
        cout << "[2]Left heavy" <<  "Node : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;
    }

    node->setLeft(rebalance(node->getLeft()));
    node->setRight(rebalance(node->getRight()));
    cout << "Rebalanced : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;

    node->updateHeight();
    node->updateWeight();

    cout << "Updated : " << node->toString() << " Left : " << left->toString() << " Right : " << right->toString() << endl;

    return node;
}

/**
 * Rotates the given node to the left in the Rope data structure.
 *
 * @param node The node to be rotated to the left.
 *
 * @return The new root node after the rotation.
 *
 * @throws None.
 */
Rope::Node* Rope::rotateLeft(Node* node)
{
    Node* newRoot = node->getRight();
    node->setRight(newRoot->getLeft());
    newRoot->setLeft(node);
    node->updateHeight();
    newRoot->updateHeight();
    return newRoot;
}

/**
 * Rotates the given node to the right in the Rope data structure.
 *
 * @param node The node to be rotated to the right.
 *
 * @return The new root node after the rotation.
 *
 * @throws None.
 */
Rope::Node* Rope::rotateRight(Node* node)
{   
    Node* newRoot = node->getLeft();
    node->setLeft(newRoot->getRight());
    newRoot->setRight(node);

    node->updateWeight();
    node->updateHeight();
    newRoot->updateWeight();
    newRoot->updateHeight();

    return newRoot;
}


/*
 * Rope Functions
 * ==============
 * These functions are used to perform various operations on the Rope data structure.
 * They are implemented as member functions of the Rope class.
*/

/**
 * Appends the given rope to the end of this rope.
 *
 * @param rope The rope to append to this rope.
 *
 * @return void
 *
 * @throws None
 */
void Rope::append(Rope& rope)
{
    Node* newRoot = merge(root, rope.root);
    root = newRoot;
}

/**
 * Appends a new node containing the given string and length to the end of the rope.
 *
 * @param str The string to append.
 * @param len The length of the string.
 *
 * @return void
 *
 * @throws None
 */
void Rope::append(const char str[], uint32_t len)
{  
    Node* newNode = new Node(str, len);

    if (root == nullptr) {
        root = newNode;
        return;
    }
    
    Node* newRoot = merge(root, newNode);
    root = newRoot;
}

/**
 * Prepends the given rope to the beginning of this rope.
 *
 * @param rope The rope to prepend to this rope.
 *
 * @return void
 *
 * @throws None
 */
void Rope::prepend(Rope& rope)
{
    if (root == nullptr) {
        root = rope.root;
        return;
    }
    
    Node* newRoot = merge(rope.root, root);
    root = newRoot;
}

/**
 * Prepends a new node containing the given string and length to the beginning of the rope.
 *
 * @param str The string to prepend.
 * @param len The length of the string.
 *
 * @return void
 *
 * @throws None
 */
void Rope::prepend(const char str[], uint32_t len)
{   
    Node* newNode = new Node(str, len);

    if (root == nullptr) {
        root = newNode;
        return;
    }
    Node* newRoot = merge(newNode, root);
    root = newRoot;
}

/**
 * Inserts a new string at the specified position in the rope.
 *
 * @param pos The position at which the string should be inserted.
 * @param str The string to be inserted.
 * @param len The length of the string.
 *
 * @throws None
 */
void Rope::insert(uint32_t pos, const char str[], uint32_t len) 
{
    if (pos == 0) { // Prepend
        prepend(str, len);
        return;
    }

    if (pos >= root->getWeight()) { // Append
        append(str, len);
        return;
    }

    cout << "splitting" << endl;
    auto splitResult = split(root, pos);
    cout << "splited into :- " << splitResult.first->toString() << " and " << splitResult.second->toString() << endl;

    Node* newNode = new Node(str, len);
    cout << "New Node created : " << newNode->toString() << endl;

    Node* leftSubtree = merge(splitResult.first, newNode);
    root = merge(leftSubtree , splitResult.second);
}

void Rope::insert(uint32_t pos, Rope& rope)
{
    if (pos == 0) { // Prepend
        prepend(rope);
        return;
    }

    if (pos >= root->getWeight()) { // Append
        append(rope);
        return;
    }

    cout << "splitting" << endl;
    auto splitResult = split(root, pos);
    cout << "splited into :- " << splitResult.first->toString() << " and " << splitResult.second->toString() << endl;

    Node* leftSubtree = merge(splitResult.first, rope.root);
    root = merge(leftSubtree , splitResult.second);
}

/**
 * Removes a character at the specified position from the rope.
 *
 * @param pos The position of the character to be removed.
 *
 * @throws None
 */
void Rope::remove(uint32_t pos)
{
    auto splitResult = split(root, pos);
    root = merge(splitResult.first, splitResult.second);
}

/**
 * Removes a range of characters from the rope starting from 'start' position to 'end' position.
 *
 * @param start The starting position of the range to be removed.
 * @param end The ending position of the range to be removed.
 *
 * @return None
 *
 * @throws None
 */
void Rope::remove(uint32_t start, uint32_t end)
{
    auto splitStart = split(root, start);
    auto splitEnd = split(splitStart.second, end - start);

    root = merge(splitStart.first, splitEnd.second);
}

/*
Rope Rope::cut(uint32_t minline, uint32_t mincol, uint32_t maxline, uint32_t maxcol)
{
    return Rope("sds",3);
}

Rope Rope::cut(uint32_t start, uint32_t end)
{

    Rope* rope = new Rope("sds",3);
    return *rope; 
}


void Rope::paste(uint32_t minline, uint32_t mincol, const Rope& r)
{

}

void Rope::load(const char filename[])
{

}

void Rope::save(const char filename[])
{

}
*/

uint32_t Rope::getLength() const
{
    return root->getWeight();
}

/*
* ROPE PRINTING FUNCTIONS
* =======================
*
* The following functions are used to print the rope in pre-order.
* The printTree() function is used to print the tree structure starting from the root node in the rope data structure.
* The toString() function is used to convert the rope object to a string representation.
*/

/**
 * Converts the Rope object to a string representation.
 *
 * @return The string representation of the Rope object.
 */
string Rope::toString() const
{
    return root->transversePreOrder();
}

/**
 * Prints the tree structure starting from the root node in the Rope data structure.
 *
 * @param None
 *
 * @return void
 *
 * @throws None
 */
void Rope::printTree()
{
    root->printTree();
}