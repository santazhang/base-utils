#pragma once

#include "basetypes.h"

namespace base {

class SparseInt {
public:
    static int buf_size(char byte1);
    static int dump(i32 val, char* buf);
//    static int dump(i64 val, char* buf);
    static i32 load_i32(const char* buf);
//    static i64 load_i64(const char* buf);
};

} // namespace base
