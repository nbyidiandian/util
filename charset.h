#ifndef UTIL_CHARSET_H
#define UTIL_CHARSET_H

#include <sys/types.h>
#include <stdint.h>
#include <string>

namespace util
{

enum {
    E_CC_GBK,
    E_CC_UTF8,
    E_CC_COUNT
};

#define CC_FLAG_IGNORE      1
#define CC_FLAG_TRANSLIT    2

typedef void *code_conv_t;

class Charset
{
public :
    Charset();

    virtual ~Charset();

    static std::string GBK2UTF8(const std::string &gbk_str);
    static std::string UTF82GBK(const std::string &utf8_str);

    static void GBK2UTF8_p(const char *gbk_str, size_t gbk_str_size,
                           char **utf8_str, size_t *utf8_str_size);
};

} /* end of namespace sp_basic */

#endif /* end of include guard: UTIL_CHARSET_H */
