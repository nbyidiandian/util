#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>

#include "charset.h"
#include "py_code.h"

using namespace std;

namespace util
{

string Charset::GBK2UTF8(const string &gbk_str)
{
    if (gbk_str.empty()) {
        return gbk_str;
    }

    size_t size = gbk_str.size() * 4 + 1;
    char *out = new char[size];

    py_gbk_to_utf8(gbk_str.c_str(), gbk_str.size(), out, size);

    string str(out);
    delete[] out;

    return str;
}

void Charset::GBK2UTF8_p(const char *gbk_str, size_t gbk_str_size,
                       char **utf8_str, size_t *utf8_str_size)
{
    size_t capacity = gbk_str_size * 4 + 1;
    *utf8_str = SPMemPool::new_vec<char>(capacity);

    int ret = py_gbk_to_utf8(gbk_str, gbk_str_size, *utf8_str, capacity);
    if (ret >= 0) {
        *utf8_str_size = ret;
    }
    else {
        *utf8_str_size = 0;
    }
}

string Charset::UTF82GBK(const string &utf8_str)
{
    if (utf8_str.empty()) {
        return utf8_str;
    }

    size_t size = utf8_str.size() * 4 + 1;
    char *out = new char[size];

    py_utf8_to_gbk(utf8_str.c_str(), utf8_str.size(), out, size);

    string str(out);
    delete[] out;

    return str;
}

} /* end of namespace sp_basic */
