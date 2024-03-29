#include <rope.hpp>
#include <cstring>
#include <string>
#include <iostream>

rope::rope() : root() {}
rope::rope(const char* s, uint32_t len) : root(s, len) {}
