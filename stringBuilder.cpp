#include "stringBuilder.hpp"

StringBuilder::StringBuilder() {
    buffer.clear();
}

void StringBuilder::append(const std::string& str) {
    buffer.append(str);
}

void StringBuilder::insert(size_t pos, const std::string& str) {
    buffer.insert(pos, str);
}

void StringBuilder::remove(size_t pos, size_t len) {
    buffer.erase(pos, len);
}

void StringBuilder::load(const std::string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file" << std::endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        append(line);
    }
}

const string& StringBuilder::toString() const {
    return buffer;
}