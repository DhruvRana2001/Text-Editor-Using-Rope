#include <string.hpp>
#include <fstream>

using namespace std;

StringEditor::StringEditor() : text() {}
StringEditor::StringEditor(const string& text) : text(text) {}

StringEditor::~StringEditor() {}

void StringEditor::append(const string& s) {
    text.append(s);
}

void StringEditor::remove(uint32_t pos) {
    text.erase(pos, 1);
}

void StringEditor::remove(uint32_t start, uint32_t end) {
    text.erase(start, end - start);
}

void StringEditor::insert(int pos, const string& s) {
    text.insert(pos, s);
}

void StringEditor::erase(int pos, int len) {
    text.erase(pos, len);
}

int StringEditor::search(const string& s) {
    return text.find(s);
}

void StringEditor::load(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        text.append(line);
    }
}

void StringEditor::save(const string& filename) {
    ofstream file(filename);
    file << text;
}

string StringEditor::str() const {
    return text;
}

int StringEditor::size() const {
    return text.size();
}
