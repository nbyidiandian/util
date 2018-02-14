#ifndef UTIL_URL_UTIL_H
#define UTIL_URL_UTIL_H

#include <string>

namespace util
{

class URLUtil
{
public:
    URLUtil();
    ~URLUtil();

public:
    static std::string decode(const std::string &str);

    static std::string encode(const std::string &str);

    /**
     * 将字符c对应的十六进制字符形式（e.g. "%BF"）写入到hex数组里
     * 注意这里没有对hex做越界检查，调用该函数时需要自行保证不会越界
     **/
    static inline void char2hex(unsigned char c, char hex[])
    {
        static unsigned char hex_table[16] = {
            '0', '1', '2', '3',
            '4', '5', '6', '7',
            '8', '9', 'A', 'B',
            'C', 'D', 'E', 'F'
        };

        unsigned char idx = 0;
        hex[0] = '%';

        idx = (c & 0xF0) >> 4;
        hex[1] = hex_table[idx];
        idx = c & 0x0F;
        hex[2] = hex_table[idx];
    }
};

}

#endif
