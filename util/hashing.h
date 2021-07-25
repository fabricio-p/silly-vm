#ifndef SILLY_HASHING_H
#define SILLY_HASHING_H
#include <stdint.h>

size_t silly_murmurHash2(const uint8_t *, int, size_t);
size_t silly_murmurHash2Aligned(const uint8_t *, int, size_t);
size_t silly_murmurHash3_x86_32(const uint8_t *, int, size_t);
size_t silly_murmurHash3_x86_128(const uint8_t *, int, size_t);
size_t silly_murmurHash3_x64_128(const uint8_t *, int, size_t);

#endif /* SILLY_HASHING_H */
