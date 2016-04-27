#include <emmintrin.h>

const char *ssechr(const char *s, char ch)
{
    __m128i zero = _mm_setzero_si128();
    __m128i cx16 = _mm_set1_epi8(ch); // (ch) replicated 16 times.
    while (1) {
        __m128i  x = _mm_loadu_si128((__m128i const *)s);
        unsigned u = _mm_movemask_epi8(_mm_cmpeq_epi8(zero, x));
        unsigned v = _mm_movemask_epi8(_mm_cmpeq_epi8(cx16, x))
                & ~u & (u - 1);
        if (v) return s + __builtin_ctz(v) - 1;
        if (u) return  NULL;
        s += 16;
    }
}

TEST(
