#include "rope.hpp"

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
        data = new char[len+1];
        std::copy(str, str + len, data);
        data[len] = '\0';
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
    if (isLeaf){
        cout << toString() << endl;
        cout << "Node has data : " << (data ? data : "null") << " and length : " << length << " and weight : " << weight << " and height : " << height << " and isLeaf : " << isLeaf << endl;
        cout << "Left : " << (left ? left->toString() : "null") << " and right : " << (right ? right->toString() : "null") << endl;
        if (data)
            delete[] data;
        data = nullptr;
        cout << "Node deleted" << endl;
    }
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
    stack<const Node*> nodeStack;
    nodeStack.push(node);

    while (!nodeStack.empty()) {
        const Node* currNode = nodeStack.top();
        nodeStack.pop();

        if (node == nullptr) { continue; }

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

        data = new char[len+1];
        copy (str, str + len, data);
        data[len] = '\0';
        setLength(len);

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

