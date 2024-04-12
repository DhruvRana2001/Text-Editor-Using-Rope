#pragma once
#include <cstdint>
#include <string>

class rope {
private:
	class node {
    private:
        uint32_t len;
        node* left;
        node* right;
        char* data;
    public:
        node() : len(0), left(nullptr), right(nullptr), data(nullptr) {}
        node(const char* s, uint32_t len) : len(len), left(nullptr), right(nullptr) {
            data = new char[len];
            memcpy(data, s, len);
        }
        ~node() {
            if (left != nullptr) {
                delete left;
            }
            if (right != nullptr) {
                delete right;
            }
            if (data != nullptr) {
                delete[] data;
            }
        }
        node(const node&& orig) : len(orig.len), left(nullptr), right(nullptr), data(nullptr) {
            left = orig.left;
            right = orig.right;
            data = new char[len];
            memcpy(data, orig.data, len);
        }
        node& operator=(const node&& orig) {
            if (this != &orig) {
                len = orig.len;
                left = orig.left;
                right = orig.right;
                data = new char[len];
                memcpy(data, orig.data, len);
            }
            return *this;
        }
    };

	node root;

	/*
		internal function to reorder the text into a single, sequential stream

		Once the text is sequential, it can be written out in a single write
		the question is, is this worth the cost?
	*/
public:
	struct mark {
        //TODO: implement
	};

	struct range {
        //TODO: implement
	};

    rope();
    rope(const char s[], uint32_t len);
	rope(const char filename[]);
	~rope();

	rope(const rope& orig) = delete;
	rope& operator =(const rope& orig) = delete;
	rope(rope&& orig);

	void append(const char s[], uint32_t len);

	//void insert(const char s[], uint32_t len);
	void remove(uint32_t pos);
	void remove(uint32_t start, uint32_t end);
	rope cut(uint32_t minline, uint32_t mincol, uint32_t maxline, uint32_t maxcol);
	void paste(uint32_t minline, uint32_t mincol, const rope& r);
	mark search(const char s[], uint32_t len) const;
	void load(const char filename[]);
	void save(const char filename[]);
};