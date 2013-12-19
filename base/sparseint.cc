#include "sparseint.h"

namespace base {

int SparseInt::buf_size(char byte1) {
    if ((byte1 & 0x80) == 0) {
        // binary: 0...
        return 1;
    } else if ((byte1 & 0xC0) == 0x80) {
        // binary: 10...
        return 2;
    } else if ((byte1 & 0xE0) == 0xC0) {
        // binary: 110...
        return 3;
    } else if ((byte1 & 0xF0) == 0xE0) {
        // binary: 1110...
        return 4;
    } else if ((byte1 & 0xF8) == 0xF0) {
        // binary: 11110...
        return 5;
    } else if ((byte1 & 0xFC) == 0xF8) {
        // binary: 111110...
        return 6;
    } else if ((byte1 & 0xFE) == 0xFC) {
        // binary: 1111110...
        return 7;
    } else if ((byte1 & 0xFF) == 0xFE) {
        // binary: 11111110...
        return 8;
    } else {
        return 9;
    }
}

int SparseInt::dump(i32 val, char* buf) {
    char* pv = (char *) &val;
    if (-64 <= val && val <= 63) {
        buf[0] = pv[0];
        buf[0] &= 0x7F;
        return 1;
    } else if (-8192 <= val && val <= 8191) {
        buf[0] = pv[1];
        buf[1] = pv[0];
        buf[0] &= 0x3F;
        buf[0] |= 0x80;
        return 2;
    } else if (-1048576 <= val && val <= 1048575) {
        buf[0] = pv[2];
        buf[1] = pv[1];
        buf[2] = pv[0];
        buf[0] &= 0x1F;
        buf[0] |= 0xC0;
        return 3;
    } else if (-134217728 <= val && val <= 134217727) {
        buf[0] = pv[3];
        buf[1] = pv[2];
        buf[2] = pv[1];
        buf[3] = pv[0];
        buf[0] &= 0x0F;
        buf[0] |= 0xE0;
        return 4;
    } else {
        buf[1] = pv[3];
        buf[2] = pv[2];
        buf[3] = pv[1];
        buf[4] = pv[0];
        if (val < 0) {
            buf[0] = 0xF7;
        } else {
            buf[0] = 0xF0;
        }
        return 5;
    }
}

// int SparseInt::dump(i64 i64, char* buf) {
//     return 0;
// }

i32 SparseInt::load_i32(const char* buf) {
    i32 val = 0;
    char* pv = (char *) &val;
    int bsize = SparseInt::buf_size(buf[0]);
    for (int i = 0; i < bsize; i++) {
        pv[i] = buf[bsize - i - 1];
    }
    if (bsize < 5) {
        pv[bsize - 1] &= 0xFF >> bsize;
        if ((pv[bsize - 1] >> (7 - bsize)) & 0x1) {
            pv[bsize - 1] |= 0xFF << (7 - bsize);
            for (int i = bsize; i < 4; i++) {
                pv[i] = 0xFF;
            }
        }
    }
    return val;
}

// i64 SparseInt::load_i64(const char* buf) {
//     i64 val = 0;
//     return val;
// }

} // namespace base
