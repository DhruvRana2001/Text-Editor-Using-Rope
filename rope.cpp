#include "rope.hpp"
#include <iostream>
#include <regex>
#include <stack>

/*
 * Rope::Node class implementation
 * Copyright (C) 2022 dhruv
*/

/**
 * Constructs a new Rope::Node object with the given string and length.
 *
 * @param str The string to initialize the node with.
 * @param len The length of the string.
 *
 * @throws None
 */
Rope::Node::Node(const char str[], uint32_t len) : left(nullptr), right(nullptr), weight(len), height(0), isLeaf(true) 
{
    if (len > 0)
    {
        data = new char[len];
        std::copy(str, str + len, data);
    }
    else
    {
        data = nullptr;
    }

    length = len;
}

/**
 * Constructs a new Rope::Node object with the given left and right nodes.
 *
 * @param left The left child node.
 * @param right The right child node.
 *
 * @return None
 *
 * @throws None
 */
Rope::Node::Node(Node* left, Node* right) : data(nullptr), length(0), isLeaf(false) 
{
    this->left = left;
    this->right = right;

    updateWeight();
    updateHeight();
}


/**
 * Destructor for Rope::Node class. Deletes the data if it's a leaf node, otherwise recursively deletes the left and right child nodes.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
Rope::Node::~Node()
{
    if (isLeaf)
        delete[] data;
    else
    {
        if (left != nullptr) 
            delete left;

        if (right != nullptr)
            delete right;
    } 
        
}

/*
* Rope::Node member functions
*   - updateHeight
*   - updateWeight
*   - balanceFactor
*/

/**
 * Updates the weight of the current node by recursively updating the weights of its children.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void Rope::Node::updateWeight()
{
    updateWeight(this);
}


/**
 * Updates the weight of the given node and its subtrees.
 *
 * @param node The node whose weight needs to be updated.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::updateWeight(Node* node)
{
    stack<Node*> nodeStack;
    nodeStack.push(node);

    while (!nodeStack.empty()) {
        Node* currNode = nodeStack.top();
        nodeStack.pop();

        if (currNode == nullptr || currNode->isLeaf || (currNode->left == nullptr && currNode->right == nullptr) ) {continue;}

        if (currNode->left != nullptr) {
            nodeStack.push(currNode->left);
        }

        if (currNode->right != nullptr) {
            nodeStack.push(currNode->right);
        }

        currNode->weight = (currNode->left != nullptr ? currNode->left->weight : 0) + (currNode->right != nullptr ? currNode->right->weight : 0);
    }
}

/**
 * Updates the height of the current node by updating the heights of its children.
 *
 * @param None
 *
 * @return None
 *
 * @throws None
 */
void Rope::Node::updateHeight()
{
    updateHeight(this);
}

/**
 * Updates the height of the given node and its subtrees.
 *
 * @param node The node whose height needs to be updated.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::updateHeight(Node* node)
{
    deque<Node*> nodeDeque;
    nodeDeque.push_back(node);

    while (!nodeDeque.empty()) {
        Node* currNode = nodeDeque.front();
        nodeDeque.pop_front();
        
        if (currNode == nullptr || currNode->isLeaf || (currNode->left == nullptr && currNode->right == nullptr) ) { continue; }

        if (currNode->left != nullptr) {
            nodeDeque.push_back(currNode->left);
        }

        if (currNode->right != nullptr) {
            nodeDeque.push_back(currNode->right);
        }

        uint32_t leftHeight = (currNode->left ? currNode->left->height : 0);
        uint32_t rightHeight = (currNode->right ? currNode->right->height : 0);

        currNode->setHeight((leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1);
    }
}

/**
 * Calculates the balance factor of the current node by calling the balanceFactor function with the current node as the argument.
 *
 * @param None
 *
 * @return int The balance factor of the current node
 *
 * @throws None
 */
int Rope::Node::balanceFactor()
{
    return balanceFactor(this);
}

/**
 * Calculates the balance factor of the given node.
 *
 * @param node The node for which the balance factor needs to be calculated.
 *
 * @return The balance factor of the node. Returns 0 if the node is null or a leaf node, or if both left and right children are null. 
 *         Returns the difference between the height of the left child and the height of the right child if both left and right children exist.
 *
 * @throws None
 */
int Rope::Node::balanceFactor(Node* node)
{
    if (node->isLeaf || node == nullptr) return 0;

    if (node->left == nullptr && node->right == nullptr) return 0;

    int leftHeight = node->left ? node->left->height : 0;
    int rightHeight = node->right? node->right->height : 0;

    return leftHeight - rightHeight;
}

/*
 * PRINTING FUNCTIONS
 * 
 * The following functions are used to print the rope node tree in pre-order.
 * The functions are defined here to avoid circular dependencies.
 *   - replaceNewLine
 *   - treeTransversePreOrder
 *   - treeTransverseNodes
 *   - toString
 *   - transversePreOrder
 *   - printTree 
*/

/**
 * Replaces all newline characters in a given string with the escape sequence "\n".
 *
 * @param str the input string to be processed
 *
 * @return the processed string with newline characters replaced by "\n"
 *
 * @throws None
 */
string replaceNewLine(const string& str)
{
    if (str.empty()) {
        return "";
    }
    
    static const regex pattern("\n");
    return regex_replace(str, pattern, "\\n");
}

/**
 * Converts the Node object to a string representation.
 *
 * @return The string representation of the Node object.
 */
string Rope::Node::toString() const
{
    string str;
    if (isLeaf && data != nullptr)
        str = string(data, length);
    else
        str = "( w:" + to_string(weight) + ", h:" + to_string(height) + " )";  

    return str;
}

/**
 * Traverses the rope node tree in pre-order and returns a string representation of the tree.
 *
 * @return The string representation of the rope node tree in pre-order.
 */
string Rope::Node::transversePreOrder() const
{
    string str;
    transversePreOrder(&str, this);
    return str;
}

/**
 * Traverses the rope node tree in pre-order and appends the string representation of each node to the given string.
 *
 * @param str Pointer to the string where the string representation of each node will be appended.
 * @param node Pointer to the current node being traversed.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::transversePreOrder(string* str, const Node* node) const
{
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf) {
        str->reserve(str->size() + node->toString().size());
        str->append(node->toString());
    }

    stack<const Node*> nodeStack;
    nodeStack.push(node);

    while (!nodeStack.empty()) {
        const Node* currNode = nodeStack.top();
        nodeStack.pop();

        if (currNode->isLeaf) {
            str->reserve(str->size() + node->toString().size());
            str->append(currNode->getData(), currNode->getLength());
        }

        if (currNode->right != nullptr) {
            nodeStack.push(currNode->right);
        }

        if (currNode->left != nullptr) {
            nodeStack.push(currNode->left);
        }
    }
}


/**
 * Prints the tree structure starting from the current node in the Rope data structure.
 *
 * @param None
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::printTree()
{
    cout << "Tree:- " << endl;
    cout << treeTransversePreOrder(this) << endl;
}

    /**
     * Traverses the rope node tree in pre-order and returns a string representation of the tree.
     *
     * @param root The root of the rope node tree.
     * @return The string representation of the rope node tree in pre-order.
     */
string Rope::Node::treeTransversePreOrder(Node* root)
{
    string* str = new string();
    
    if (root == nullptr) {
        return str->c_str();
    }

    if (root->isLeaf) {
        str->append("\n");
        str->append("( w:" + to_string(getWeight()) + ", h:" + to_string(getHeight()) + " ) ");
        str->append(replaceNewLine(root->toString()));
    }
    else {
        str->append("\n");
        str->append(replaceNewLine(root->toString()));
    }

    string pointerRight = "\\__ [R]";
    string pointerLeft = (root->right != nullptr) ? "|-- [L]" : "\\__ [L]";

    treeTransverseNodes(str, "", pointerLeft, root->left, root->right != nullptr);
    treeTransverseNodes(str, "", pointerRight, root->right, false);

    str->append("\n");

    return str->c_str();
}

/**
 * Recursive function to traverse the nodes of the rope tree and build a string representation.
 *
 * @param str Pointer to the string where the representation is appended.
 * @param padding The padding string for current level in the tree.
 * @param pointer The pointer representation for the current node.
 * @param node The current node being traversed.
 * @param hasRightChild Indicates if the current node has a right child.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::treeTransverseNodes(string *str, string padding, string pointer, Node* node, bool hasRightChild)
{

    if (node == nullptr) {
        return;
    }

    str->append("\n");
    str->append(padding);
    str->append(pointer);
    
    if (node->getIsLeaf()) {
        str->append("( w:" + to_string(node->getWeight()) + ", h:" + to_string(node->getHeight()) + " ) ");
        str->append(replaceNewLine(node->toString()));
    }
    else {
        str->append(node->toString());
    }

    
    string paddingBuilder = padding;
    paddingBuilder.append(hasRightChild ? "|   " : "    ");

    string paddingForBoth = paddingBuilder;
    string pointerRight = "\\__ [R]";
    string pointerLeft = (node->right != nullptr) ? "|-- [L]" : "\\__ [L]";

    treeTransverseNodes(str, paddingForBoth, pointerLeft, node->left, node->right != nullptr);
    treeTransverseNodes(str, paddingForBoth, pointerRight, node->right, false);
}


/*****************************************************************
 * GETTERS AND SETTERS
 * These methods are used to get and set the properties of the
 * Rope::Node object.
 *****************************************************************/

/**
 * Retrieves the weight of the current Rope::Node object.
 *
 * @param None
 *
 * @return The weight of the node.
 *
 * @throws None
 */
uint32_t Rope::Node::getWeight() const
{
    return weight;
}

/**
 * Sets the weight of the Rope::Node object.
 *
 * @param w The new weight value to be set.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setWeight(uint32_t w)
{
    this->weight = w;
}

/**
 * Retrieves the height of the current Rope::Node object.
 *
 * @return The height of the node.
 */
uint32_t Rope::Node::getHeight() const
{
    return height;
}

/**
 * Sets the height of the Rope::Node object.
 *
 * @param h The new height value to be set.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setHeight(uint32_t h)
{
    this->height = h;
}

/**
 * Retrieves the left child node of the current Rope::Node object.
 *
 * @return A pointer to the left child node if it exists, otherwise nullptr.
 */
Rope::Node* Rope::Node::getLeft() const
{
    if (!isLeaf) {
        return left;
    }
    else {
        return nullptr;
    }
}

/**
 * Sets the left child node of the current Rope::Node object.
 *
 * @param node The new left child node to be set.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setLeft(Node* node) // make sure to update height and weight after setting
{
    left = node;
}

/**
 * Retrieves the right child node of the current Rope::Node object.
 *
 * @return A pointer to the right child node if it exists, otherwise nullptr.
 */
Rope::Node* Rope::Node::getRight() const
{
    if (!isLeaf) {
        return right;
    }
    else {
        return nullptr;
    }
}  

/**
 * Sets the right child node of the current Rope::Node object.
 *
 * @param node The new right child node to be set.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setRight(Node* node)  // make sure to update height and weight after setting
{
    right = node;
}

/**
 * Retrieves the data stored in the current Rope::Node object.
 *
 * @return A pointer to the data if the node is a leaf node, otherwise nullptr.
 */
char* Rope::Node::getData() const
{
    if (getIsLeaf()) {
        return data;
    }
    else {
        return nullptr;
    }
}

/**
 * Sets the data of the Rope::Node object.
 *
 * @param str The string to set as the data.
 * @param len The length of the string.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setData(const char* str, uint32_t len)
{
    if (getIsLeaf()) {
        if (data != nullptr) {
            delete[] data;
        }
        data = new char[len];
        if (data != nullptr) {
            std::copy(str, str + len, data);
            setLength(len);
        }
        else {
            // handle memory allocation failure 
            throw std::bad_alloc();
        }
    }
    else {
        data = nullptr;
        setLength(0);
    }
}

/**
 * Retrieves the length of the Rope::Node object.
 *
 * @return The length of the node if it is a leaf node, otherwise 0.
 */
uint32_t Rope::Node::getLength() const
{
    return isLeaf ? length : 0;
}

/**
 * Sets the length of the Rope::Node object.
 *
 * @param len The length to set.
 *
 * @return void
 *
 * @throws None
 */
void Rope::Node::setLength(uint32_t len)
{
    if (!isLeaf) return;

    length = len;
    weight = len;
}

/**
 * Retrieves the value of the 'isLeaf' flag for the current Rope::Node object.
 *
 * @return true if the node is a leaf node, false otherwise.
 */
bool Rope::Node::getIsLeaf() const
{
    return isLeaf;
}


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
    adjustParameters(len);

    vector<pair<char*, uint32_t>> chunks = splitTextIntoChunks(str, len);

    for (auto& chunk : chunks) {
        if (root == nullptr) {
            root = new Node(chunk.first, chunk.second);
            continue;
        }
        root = merge(root, new Node(chunk.first, chunk.second));
        delete[] chunk.first; // delete the memory allocated for the chunk
    }
}

Rope::Rope(Node* left, Node* right)
{
    adjustParameters(left->getWeight() + right->getWeight());
    root = new Node(left, right);
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

Rope::Rope() : root(nullptr) { adjustParameters(0); }

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
void Rope::append(const Rope& rope)
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
    
    vector<pair<char*, uint32_t>> chunks = splitTextIntoChunks(str, len);

    for (auto& chunk : chunks) {
        if (chunk.first == nullptr) {
            continue;
        }

        Node* newNode = new Node(chunk.first, chunk.second);

        if (root == nullptr) {
            root = newNode;
            delete[] chunk.first;
            continue;
        }
        root = merge(root, newNode);
        delete[] chunk.first;
    }
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
void Rope::prepend(const Rope& rope)
{
    if (root == nullptr) {
        root = rope.root;
        return;
    }

    root = merge(rope.root, root);
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
    vector<pair<char*, uint32_t>> chunks = splitTextIntoChunks(str, len);

    for (auto& chunk : chunks) {
        if (chunk.first == nullptr) {
            continue;
        }

        Node* newNode = new Node(chunk.first, chunk.second);

        if (root == nullptr) {
            root = newNode;
            delete[] chunk.first;
            continue;
        }

        root = merge(newNode, root);
        delete[] chunk.first;
    }
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

    vector<pair<char*, uint32_t>> chunks = splitTextIntoChunks(str, len);

    uint32_t adjustedPos, currentPos = 0;

    for (auto& chunk : chunks) {
        if (chunk.first == nullptr) {
            continue;
        }
        
        adjustedPos = pos + currentPos;

        if (adjustedPos == 0) { // Prepend
            prepend(chunk.first, chunk.second);
            delete[] chunk.first;
            continue;
        }

        else if (adjustedPos >= root->getWeight()) { // Append
            append(chunk.first, chunk.second);
            delete[] chunk.first;
            continue;
        }

        else {
            cout << "splitting" << endl;
            auto splitResult = split(root, adjustedPos);
            cout << "splited into :- " << splitResult.first->toString() << " and " << splitResult.second->toString() << endl;

            Node *newNode = new Node(chunk.first, chunk.second);
            cout << "New Node created : " << newNode->toString() << endl;

            Node *leftSubtree = merge(splitResult.first, newNode);
            root = merge(leftSubtree , splitResult.second);
        }

        currentPos += chunk.second;
    }
}

/**
 * Inserts a rope at the specified position in the rope.
 *
 * @param pos The position at which the rope should be inserted.
 * @param rope The rope to be inserted.
 *
 * @return None
 *
 * @throws None
 */
void Rope::insert(uint32_t pos, const Rope& rope)
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
void Rope::remove(uint32_t start, uint32_t length)
{
    auto splitStart = split(root, start);
    auto splitEnd = split(splitStart.second, length - start);

    root = merge(splitStart.first, splitEnd.second);
}


/**
 * Cuts a range of characters from the rope starting from 'start' position to 'end' position.
 *
 * @param start The starting position of the range to be cut.
 * @param end The ending position of the range to be cut.
 *
 * @return A new Rope object containing the cut range of characters.
 *
 * @throws None
 */
Rope Rope::cut(uint32_t start, uint32_t end)
{
    auto splitResult = split(root, start);
    auto splitEnd = split(splitResult.second, end - start);

    return Rope (splitResult.second, splitEnd.first);
}


/**
 * Inserts the contents of the given rope `r` at the specified position `pos` in the current rope.
 *
 * @param pos The position at which the contents of `r` should be inserted.
 * @param r The rope whose contents should be inserted.
 *
 * @throws None
 */
void Rope::paste(uint32_t pos, const Rope& r)
{
    insert(pos, r);
}



void Rope::load(const char filename[])
{
    //file = fopen(filename, "r");

}

void Rope::save(const char filename[])
{

}

uint32_t Rope::getLength() const
{
    return root->getWeight();
}

/*
* ROPE HELPER FUNCTIONS
* =====================
* 
*/

/**
 * Sets the chunk size for the Rope object.
 *
 * @param chunkSize The size of each chunk in the rope.
 *
 * @return void
 *
 * @throws None
 */
void Rope::setChunkSize(uint32_t chunkSize)
{
    this->chunkSize = chunkSize;
}

/**
 * Adjusts the chunk size based on the file size.
 *
 * @param fileSize The size of the file in bytes.
 *
 * @throws None
 */
void Rope::adjustParameters(uint32_t fileSize)
{   
    if (fileSize <= 1000) { // Tiny file or just maunally making the rope
        chunkSize = 10;
    }
    else if (fileSize <= 1000000) { // Small file
        chunkSize = 100;
    } else if (fileSize <= 10000000) { // Medium file
        chunkSize = 1000;
    } else if (fileSize <= 100000000) { // Large file
        chunkSize = 10000;
    } else { // Huge file
        chunkSize = 100000;
    }
}

vector<pair<char*, uint32_t>> Rope::splitTextIntoChunks(const char* text, uint32_t length)
{
    vector<pair<char*, uint32_t>> chunks;

    uint32_t startIndex = 0;

    while (startIndex < length) {
        uint32_t endIndex = min(startIndex + chunkSize, length);

        int newlinePos = std::string(text + startIndex, endIndex - startIndex).find('\n');
        if (newlinePos != std::string::npos) {
            endIndex = startIndex + newlinePos + 1; // Include the newline character
        }

        char* chunk = new char[endIndex - startIndex + 1];
        memcpy(chunk, text + startIndex, endIndex - startIndex);
        chunk[endIndex - startIndex] = '\0';

        chunks.push_back(make_pair(chunk, endIndex - startIndex));

        startIndex = endIndex;

        if (startIndex >= length) {
            break;
        }
    }
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
