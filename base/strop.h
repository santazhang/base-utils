#pragma once

namespace base {

#define streq(a, b) (strcmp((a), (b)) == 0)

bool startswith(const char* str, const char* head);
bool endswith(const char* str, const char* head);

} // namespace base
