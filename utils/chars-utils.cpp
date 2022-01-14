#include "chars-utils.h"
#include <string.h>

namespace nbc {
namespace utils {
int fromUtf8( unsigned char b, unsigned char* src, unsigned char* end, unsigned char* dst_buf) {
    if (b < 0x80) {
        if (dst_buf) {
            *dst_buf = b;
        }
        return 1;
    }

    int charsNeeded;
    unsigned char min_uc;
    unsigned char uc;

    if (b <= 0xC1) {
        // 除了 US-ASCII 字符， UTF-8 字符的第一个字节至少是 0xC0
        // 然而， 所有 0xC0 和 0xC1 为第一字节的只能产生超长序列
        return -1;
    } else if (b < 0xe0) {
        charsNeeded = 2;
        min_uc = 0x80;
        uc = b & 0x1f;
    } else if (b < 0xf0) {
        charsNeeded = 3;
        min_uc = 0x800;
        uc = b & 0x0f;
    } else if (b < 0xf5) {
        charsNeeded = 4;
        min_uc = 0x10000;
        uc = b & 0x07;
    } else {
        // 最后一个 Unicode 字符编码为 U+10FFFF
        // 它的 UTF-8 编码为 "\xF4\x8f\xBF\xBF"
        // 第一字节最大是 0xF4
        return -1;
    }
    if (dst_buf) {
        memcpy(dst_buf, src, charsNeeded);
    }
    return charsNeeded;
}

int truncationUtf8(unsigned char* src, size_t src_len, size_t dst_len, unsigned char* dst = nullptr) {
    if (dst_len >= src_len) {
        if (dst) {
            memcpy(dst, src, src_len);
        }
        return src_len;
    }

    int charsLength = 0;

    unsigned char* cur = src;
    unsigned char* end = cur + src_len;

    while (cur != end) {
        int ret = fromUtf8(*cur, cur, end);
        if (ret < 0) {
            return ret;
        }
        if (charsLength + ret <= dst_len) {
            charsLength += ret;
        } else {
            break;
        }
    }

    if (dst) {
        memcpy(dst, src, charsLength);
    }
    return charsLength;
}
}
}