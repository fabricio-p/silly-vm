#ifndef SOAP_UTIL_H
#define SOAP_UTIL_H
#include "silly_bytearray.h"
#include <stdint.h>
#include <malloc.h>

SillyByteArray silly_uleb128_encode32(uint32_t);
SillyByteArray silly_sleb128_encode32(int32_t);
SillyByteArray silly_uleb128_encode64(uint64_t);
SillyByteArray silly_sleb128_encode54(int64_t);

uint32_t silly_uleb128_decode32(uint8_t *);
int32_t  silly_sleb128_decode32(uint8_t *);
uint64_t silly_uleb128_decode64(uint8_t *);
int64_t  silly_sleb128_decode64(uint8_t *);

#endif /* SOAP_UTIL_H */
