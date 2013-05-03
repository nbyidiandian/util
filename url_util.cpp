#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "url_util.h"

using namespace std;

namespace util
{

URLUtil::URLUtil()
{ }

URLUtil::~URLUtil()
{ }

string URLUtil::decode(const string &str)
{
    const char *src = str.c_str();
    int nlen = str.length();

    char *buf = (char *)malloc(nlen+1);
    assert(buf);

    char *dst = buf;
    for (int i = 0; i <= nlen; i++) {
        if (src[i] == '+') {
            *dst++ = ' ';
        }
        else if (src[i] == '%') {
            i++;
            int hi = toupper(src[i]) - '0';
            if (hi < 0) {
                break;
            }
            else if (hi > 9) {
                hi = hi + '0' - 'A' + 10;
            }
            if (hi >= 16) {
                break;
            }
            i++;
            int lo = toupper( src[i] ) - '0';
            if (lo < 0) {
                break;
            }
            else if (lo > 9) {
                lo = lo + '0' - 'A' + 10;
            }
            if (lo >= 16) {
                break;
            }

            *dst++ = ((unsigned char)hi<<4) + lo ;
        }
        else {
            *dst++ = src[i];
        }
    }
    string ss(buf);
    free(buf);
    return ss;
}

string URLUtil::encode(const string &str)
{
    const char *src = str.c_str();
    int srclen = str.size();

    // enough buffer for output
    // a character will be encoded to 3 char at most
    char dest[srclen * 3 + 1];

    static unsigned char encode_tab[256] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };

    int dcnt = 0;
    unsigned char ch = '\0';

    for (int i = 0; i < srclen; i++) {
        ch = src[i];

        if (ch == ' ') {
            dest[dcnt++] = '+';
        }
        else if (encode_tab[ch] == 1) {
            char2hex(ch, dest+dcnt);
            dcnt += 3;
        }
        else {
            dest[dcnt++] = ch;
        }
    }
    dest[dcnt] = '\0';

    return string(dest);
}

}

