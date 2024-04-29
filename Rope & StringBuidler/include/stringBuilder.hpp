#ifndef STRINGBUILDER_HPP
#define STRINGBUILDER_HPP

#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class StringBuilder {
private:
    string buffer;

public:
    StringBuilder();
    ~StringBuilder();

    void append(const string& str);

    void insert(size_t pos, const string& str);

    void remove(size_t pos, size_t len);

    void load (const string& filename);

    const string& toString() const;
};

#endif