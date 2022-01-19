#ifndef CHARS_UTILS_H
#define CHARS_UTILS_H

namespace nbc {
namespace utils {
int fromUtf8( unsigned char b, unsigned char* src, unsigned char* end, unsigned char* dst_buf = nullptr);
// 截断utf8字符串到指定字节数，同时如果遇到半个文字编码则舍弃
int truncationUtf8(unsigned char* src, unsigned int src_len, unsigned int dst_len, unsigned char* dst = nullptr);
}
}
#endif // CHARS_UTILS_H
