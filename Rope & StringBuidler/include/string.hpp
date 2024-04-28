#pragma once
#include <string>
#include <cstdint>

using namespace std;

class StringEditor {
private:
    string text;
public:
    StringEditor();
    StringEditor(const string& text);
    ~StringEditor();

    void append(const string& s);

    void remove(uint32_t pos);
    void remove(uint32_t start, uint32_t end);



    void insert(int pos, const string& s);
    
    void erase(int pos, int len);
    
    int search(const string& s);

    void load(const string& filename);
    void save(const string& filename);

    string str() const;
    int size() const;
};

