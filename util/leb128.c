#include "leb128.h"

SillyByteArray silly_uleb128_encode32(uint32_t value) {
	SillyByteArray encoded;
	silly_bytearray_init(&encoded, 0);
	uint8_t byte = 0;
	while(1) {
		/* if (i > length) {
			encoded = (uint8_t *)realloc(encoded, (length <<= 1));
		} */
		byte = value & 0x7f;
		value >>= 7;
		if (value == 0) {
			silly_bytearray_append(&encoded, byte);
			return encoded;
		}
		silly_bytearray_append(&encoded, 0x80 | byte);
	}
}
// TODO: Make everything use SillyByteArrays
SillyByteArray silly_sleb128_encode32(int32_t value) {
	SillyByteArray encoded;
	silly_bytearray_init(&encoded, 0);
	uint8_t byte = 0;
	while(1) {
		byte = value & 0x7f;
		value >>= 7;
		if ((value == 0 && (byte & 0x40) == 0) ||
				(value == -1 && (byte & 0x40) != 0)) {
			silly_bytearray_append(&encoded, byte);
			return encoded;
		}
		silly_bytearray_append(&encoded, 0x80 | byte);
	}
}
SillyByteArray silly_uleb128_encode64(uint64_t value) {
	SillyByteArray encoded;
	silly_bytearray_init(&encoded, 0);
	uint8_t byte = 0;
	while(1) {/*
		if (i == length - 1) {
			encoded = (uint8_t *)realloc(encoded, (length <<= 1));
		}*/
		byte = value & 0x7f;
		value >>= 7;
		if (value == 0) {
			silly_bytearray_append(&encoded, byte);
			return encoded;
		}
		silly_bytearray_append(&encoded, 0x80 | byte);
	}
}
SillyByteArray silly_sleb128_encode64(int64_t value) {
	SillyByteArray encoded;
	silly_bytearray_init(&encoded, 0);
	uint8_t byte = 0;
	while (1) {/*
		if (i == length - 1) {
			encoded = (uint8_t *)realloc(encoded, (length <<= 1));
		}*/
		byte = value & 0x7f;
		value >>= 7;
		if ((value == 0 && (value & 0x40) == 0) ||
				(value == -1 && (value & 0x40) != 0)) {
			silly_bytearray_append(&encoded, byte);
			return encoded;
		}
		silly_bytearray_append(&encoded, 0x80 | byte);
	}
}

uint32_t silly_uleb128_decode32(uint8_t *encoded) {
	uint32_t value = 0;
	int i = 0;
	uint8_t byte;
	do {
		byte = *(encoded + i);
		value += (byte & 0x7f) << (i * 7);
	} while ((byte >= 0x80) && ++i);
	return value;
}
int32_t silly_sleb128_decode32(uint8_t *encoded) {
	int32_t value = 0;
	int i = 0;
	uint8_t byte;
	do {
		byte = *(encoded + i);
		value += (byte & 0x7f) << (i * 7);
	} while ((byte >= 0x80) && ++i);
	if ((byte & 0x40) != 0) {
		value |= - (1 << ((i * 7) + 7));
	}
	return value;
}
uint64_t silly_uleb128_decode64(uint8_t *encoded) {
	uint64_t value = 0;
	int i = 0;
	uint8_t byte;
	do {
		byte = *(encoded + i);
		value += (byte & 0x7f) << (i * 7);
	} while ((byte >= 0x80) && ++i);
	return value;
}
int64_t silly_sleb128_decode64(uint8_t *encoded) {
	int64_t value = 0;
	int i = 0;
	uint8_t byte;
	do {
		byte = *(encoded + i);
		value += (byte & 0x7f) << (i * 7);
	} while ((byte >= 0x80) && ++i);
	if ((byte & 0x40) != 0) {
		value |= - (1 << ((i * 7) * 7));
	}
	return value;
}
