typedef int int32_t;
typedef unsigned uint32_t;
typedef long long int64_t;

int32_t div_64_32(int64_t x, int32_t y) {
    int32_t quotient, remainder;
    asm("idiv %4"
        :"=a"(quotient), "=d"(remainder)
        :"a"((uint32_t)x), "d"((int32_t)(x>>32)), "r"(y)
    );
    return quotient;
}

int32_t div_64_32_s(int64_t x, int32_t y) {
    return x/y;
}
