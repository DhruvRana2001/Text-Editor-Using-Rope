#pragma once
#include <cstdint>
#include <string>
#include <iostream>
#include <queue>
#include <memory>
#include <regex>

using namespace std;

class Rope {
private:
    class Node {
    private:

        char *data;
        uint32_t length;

        Node* left;
        Node* right;
        
        uint32_t weight;// Weight of the node (sum of left + right children) or lenght of the data if the node is a leaf
        uint32_t height;// Height of the node
        bool isLeaf;// Flag to differentiate between leaf and internal nodes

        void setWeight(uint32_t weight);    
        void updateWeight(Node* node);

        void setHeight(uint32_t height);
        void updateHeight(Node* node);

        int balanceFactor(Node* node);
        
        void transversePreOrder(string* str, const Node* node) const;
        string treeTransversePreOrder(Node* node);
        void treeTransverseNodes(string *str, string padding, string pointer, Node* node, bool hasRightChild);
        
    public:
        Node(const char str[], uint32_t len);
        Node(Node* left, Node* right);
        ~Node();

        uint32_t getWeight() const;
        void updateWeight();

        uint32_t getHeight() const;
        void updateHeight();

        uint32_t getLength() const;
        void setLength(uint32_t len);

        int balanceFactor();

        Node* getLeft() const;
        void setLeft(Node* node);

        Node* getRight() const;
        void setRight(Node* node);

        char* getData() const;
        void setData(const char* str, uint32_t len);

        bool getIsLeaf() const;

        string toString() const;

        string transversePreOrder() const;

        void printTree();
    };

    Node* root;

    std::pair<Node*, Node*> split(Node* node, uint32_t pos) ;
    Node* merge(Node* left, Node* right);

    Node* rebalance(Node* node);
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);

    uint32_t chunkSize; // Adjust chunk size based on file size
    uint32_t rebalanceThreshold; // Adjust threshold based on file size
    std::vector<Node*> batchedOperations;

    void setChunkSize(uint32_t size);
    void setRebalanceThreshold(uint32_t threshold);

    void adjustParameters(uint32_t fileSize);

public:
    Rope();
    Rope(const char str[], uint32_t len);
    Rope(const char filename[]);
    ~Rope();

    Rope(const Rope& orig) = delete;
	Rope& operator =(const Rope& orig) = delete;

    void append(Rope& rope);
    void append(const char s[], uint32_t len);

    void prepend(Rope& rope);
    void prepend(const char s[], uint32_t len);

    void insert(uint32_t pos, Rope& rope);
	void insert(uint32_t pos,const char s[], uint32_t len);

	void remove(uint32_t pos);
	void remove(uint32_t start, uint32_t end);

	Rope cut(uint32_t minline, uint32_t mincol, uint32_t maxline, uint32_t maxcol);
    Rope cut(uint32_t start, uint32_t end);

	void paste(uint32_t start, uint32_t end, const Rope& r);

	//mark search(const char s[], uint32_t len) const;
    
	void load(const char filename[]);
	void save(const char filename[]);

    uint32_t getLength() const;

    string toString() const;

    void printTree();
};