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
    adjustParameters(len);

    vector<pair<char*, uint32_t>> chunks = splitTextIntoChunks(str, len);

    for (auto& chunk : chunks) {
        
        if (chunk.first == nullptr) {
            continue;
        }

        Node *newNode = new Node(chunk.first, chunk.second);

        if (root == nullptr) {
            root = newNode;
            continue;
        }
        root = merge(root, newNode);
        delete[] chunk.first; // delete the memory allocated for the chunk
    }
}

Rope::Rope(Node* node) : root(node) {
    adjustParameters(node->getWeight());
    
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
Rope::Rope(const char filename[]) : root(nullptr)
{
    // TODO: Read file and create rope
    ifstream file (filename, ios::ate | ios::binary | ios::in);

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    adjustParameters(file.tellg());

    load(filename);
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
    
    Node* newNode = new Node(left, right);
    

    newNode->updateWeight();
    newNode->updateHeight();

    
    
    if (abs(int(left->getHeight() - right->getHeight())) > 1) {
        //rebalancing - Balancing is only performed when the height difference between subtrees exceeds a certain threshold, reducing unnecessary balancing operations.
        
        newNode = rebalance(newNode);
    }

    
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
            
            return {left, right};
        }
    }
    else {
        
        uint32_t leftWeight = node->getLeft()->getWeight();
        
        if (pos <= leftWeight) {
            
            auto [left, right] = split(node->getLeft(), pos);
            
            
            return {left, merge(right, node->getRight())};
        }
        else {
            
            auto [left, right] = split(node->getRight(), pos - leftWeight);
            
            
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
    
    if (balance > 1) { // Left heavy
        if (left->balanceFactor() < 0)
            node->setLeft(rotateLeft(left));

        
        node = rotateRight(node);
        
    }
    else if (balance < -1) { // Right heavy
        if (right->balanceFactor() > 0)
            node->setRight(rotateRight(right));

        
        node = rotateLeft(node);
        
    }

    node->setLeft(rebalance(node->getLeft()));
    node->setRight(rebalance(node->getRight()));
    

    node->updateHeight();
    node->updateWeight();

    

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

    uint32_t currentPos = 0;

    for (auto& chunk : chunks) {
        if (chunk.first == nullptr) {
            continue;
        }
        
        uint32_t adjustedPos = pos + currentPos;

        

        if (adjustedPos == 0) { // Prepend
            prepend(chunk.first, chunk.second);
        }
        else if (adjustedPos >= root->getWeight()) { // Append
            append(chunk.first, chunk.second);
            
        }
        else {
            
            auto splitResult = split(root, adjustedPos);
            

            Node *newNode = new Node(chunk.first, chunk.second);
            

            Node *leftSubtree = merge(splitResult.first, newNode);
            root = merge(leftSubtree , splitResult.second);
        }

        currentPos += chunk.second;

        

        delete[] chunk.first;

        
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

    
    auto splitResult = split(root, pos);
    

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
    
    if (splitStart.second == nullptr) {
        root = splitStart.first;
        
        return;
    }


    auto splitEnd = split(splitStart.second, length);
    

    if (splitEnd.second == nullptr) {
        root = splitStart.first;
        
        
        return;
    }
    if (splitStart.first == nullptr) {
        root = splitEnd.second;
        
        
        return;
    }
    
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
Rope* Rope::cut(uint32_t start, uint32_t end)
{
    auto splitResult = split(root, start);
    
    //auto splitEnd = split(splitResult.second, end - start);
    auto splitEnd = split(splitResult.second, end);
    
    
    Rope* rope = new Rope(splitEnd.first);

    
    rope->printTree();

    return rope;
}


/**
 * Inserts the contents of the given rope `r` at the specified position `pos` in the current rope.
 *
 * @param pos The position at which the contents of `r` should be inserted.
 * @param r The rope whose contents should be inserted.
 *
 * @throws None
 */
void Rope::paste(uint32_t pos, const Rope* r)
{
    insert(pos, *r);
}



void Rope::load(const char filename[])
{
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        append(line.c_str(), line.length());
    }

    file.close();
    return;
}

void Rope::save(const char filename[])
{
    ofstream file (filename);
    string str = toString();

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
    }
    else {
        file << str;
        file.close();
        cout << "Data has been written to : " << filename << endl;
    }

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
    if (fileSize <= 100) { // Tiny file or just maunally making the rope
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

    /*
    while (startIndex < length) {
        uint32_t endIndex = min(startIndex + chunkSize, length);

        size_t newlinePos = std::string(text + startIndex, endIndex - startIndex).find('\n');
        if (newlinePos != std::string::npos) {
            endIndex = startIndex + newlinePos + 1; // Include the newline character
        }

        char* chunk = new char[endIndex - startIndex + 1];
        memcpy(chunk, text + startIndex, endIndex - startIndex);
        chunk[endIndex - startIndex] = '\0';

        cout << "Chunk : " << chunk << " " << endIndex - startIndex << endl;

        chunks.push_back(make_pair(chunk, endIndex - startIndex));

        startIndex = endIndex;

        if (startIndex >= length) {
            break;
        }
    }
    */

    while (startIndex < length) {
        uint32_t endIndex;
        char* chunk;
        size_t newlinePos = std::string(text + startIndex, length - startIndex).find('\n');
        if (newlinePos != std::string::npos) {
            endIndex = startIndex + newlinePos + 1; // Include the newline character
            chunk = new char[endIndex - startIndex + 1];
            memcpy(chunk, text + startIndex, endIndex - startIndex);
            chunk[endIndex - startIndex] = '\0';
        }
        else {
            endIndex = length;
            chunk = new char[endIndex - startIndex + 1];
            memcpy(chunk, text + startIndex, endIndex - startIndex);
            chunk[endIndex - startIndex] = '\0';
        }

        chunks.push_back(make_pair(chunk, endIndex - startIndex));
        startIndex = endIndex;

        if (startIndex >= length) {
            break;
        }

    }
    return chunks;
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
