#pragma once

#include <string.h>
#include <string>
#include <vector>

namespace base {

#define streq(a, b) (strcmp(a, b) == 0)

bool startswith(const char* str, const char* head);
bool endswith(const char* str, const char* head);

std::vector<std::string> strsplit(const std::string& str, const char sep = ' ');

} // namespace base
