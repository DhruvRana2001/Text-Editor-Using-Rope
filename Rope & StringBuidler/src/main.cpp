#include <iostream>
#include "rope.hpp"

using namespace std;

uint32_t getCharSize(const char* str) {
    uint32_t size = 0;
    while (str[size] != '\0') {
        size++;
    }
    return size;
}

int main() {
    const char* str= "Hello, World!";
    Rope* rope = new Rope(str, getCharSize(str));

    cout << rope->toString() << endl;

    rope->printTree();

    rope->append(" Trying to append some stuff", getCharSize(" Trying to append some stuff"));

    rope->printTree();

    rope->append(" more \n stuff", getCharSize("more \n stuff"));

    rope->printTree();

    rope->append(" testing is it working", getCharSize(" testing is it working"));

    rope->printTree();

    rope->append(" more \nlets stuff", getCharSize(" more \nlets stuff"));

    rope->printTree();

    rope->append(" testing is it working mess", getCharSize(" testing is it working mess"));

    rope->printTree();

    rope->append("one more append", getCharSize("one more append"));

    rope->printTree();

    rope->append("one more append [1]", getCharSize("one more append [1]"));

    rope->printTree();

    rope->append("one more append [\n]", getCharSize("one more append [\n]]"));

    rope->printTree();

    //rope->prepend("Trying to insert some stuff ", getCharSize("Trying to insert some stuff "));
    rope->insert(0, "Trying to insert some stuff ", getCharSize("Trying to insert some stuff "));

    rope->printTree();

    //rope->prepend(" Lets try to insert. ", getCharSize(" Lets try to insert. "));
    rope->insert(0, " Lets try to insert. ", getCharSize(" Lets try to insert. "));

    rope->printTree();

    rope->insert(52, " @lets see if we can insert in the middle@ ", getCharSize(" @lets see if we can insert in the middle@ "));

    rope->printTree();

    rope->insert(49+3+3, " &lets see if we can insert in the middle& ", getCharSize(" &lets see if we can insert in the middle& "));

    rope->printTree();
    
    cout << "Rope to string:- " << endl;
    cout << rope->toString() << endl;

    delete rope;
    return 0;
}