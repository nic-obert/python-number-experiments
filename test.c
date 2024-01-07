#include <stdio.h>
#include <inttypes.h>
#include <math.h>

typedef unsigned __int128 uint128_t;

/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

static int print_u128_u(uint128_t u128) {
    int rc;
    if (u128 > UINT64_MAX) {
        uint128_t leading  = u128 / P10_UINT64;
        uint64_t  trailing = u128 % P10_UINT64;
        rc = print_u128_u(leading);
        rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
    } else {
        uint64_t u64 = u128;
        rc = printf("%" PRIu64, u64);
    }
}

int main() {
    printf("int %lu, max %lu, eq %u\n", sizeof(int), pow(2, sizeof(int)*8), (unsigned int)~0);
    printf("long %lu\n", sizeof(long));
    printf("128-bit int %lu\n", sizeof(__int128_t));
    printf("max 128-bit int ");
    print_u128_u((uint128_t)~0);
    printf("\n");
}