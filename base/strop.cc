#include <string.h>

#include "strop.h"

namespace base {

bool startswith(const char* str, const char* head) {
    size_t len_str = strlen(str);
    size_t len_head = strlen(head);
    if (len_head > len_str) {
        return false;
    }
    return strncmp(str, head, len_head) == 0;
}

bool endswith(const char* str, const char* tail) {
    size_t len_str = strlen(str);
    size_t len_tail = strlen(tail);
    if (len_tail > len_str) {
        return false;
    }
    return strncmp(str + (len_str - len_tail), tail, len_tail) == 0;
}

std::vector<std::string> strsplit(const std::string& str, const char sep /* =? */) {
    std::vector<std::string> split;
    size_t begin, end;
    begin = str.find_first_not_of(sep);
    while ((end = str.find(sep, begin)) != std::string::npos) {
        split.push_back(str.substr(begin, end - begin));
        begin = str.find_first_not_of(sep, end);
    }
    if (begin != std::string::npos && begin < str.size()) {
        split.push_back(str.substr(begin));
    }
    return split;
}

} // namespace base
