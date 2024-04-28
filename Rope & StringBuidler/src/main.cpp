#include <iostream>
#include <cstring>
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

    Rope* rope = new Rope(str, strlen(str));
    cout << rope->toString() << endl;
    rope->printTree();

    str = " Trying to append some stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " more \n stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " testing is it working";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " more \nlets stuff";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " testing is it working mess";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append [1]";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " one more append [\n]";
    rope->append(str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = "Trying to insert some stuff ";
    //rope->prepend(str, strlen(str));
    rope->insert(0,str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " Lets try to insert. ";
    //rope->prepend(str, strlen(str));
    rope->insert(0,str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " @lets see if we can insert in the middle@ ";
    rope->insert(52, str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;

    str = " &l-e-t-s s-e-e i-f w-e c-a-n i-n-s-e-r-t i-n t-h-e m-i-d-d-l-e& ";
    rope->insert(49+3+3, str, strlen(str));
    rope->printTree();
    cout << rope->toString() << endl;
    
    cout << "Rope to string:- " << endl;
    cout << rope->toString() << endl;

    delete rope;
    return 0;
}