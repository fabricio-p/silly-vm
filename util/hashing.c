#include "hashing.h"

size_t silly_murmurHash2(const uint8_t *data, int len, size_t seed) {
	size_t magic = 0x5bd1e995;
	const int r = 24;
	size_t hash = seed ^ len;
	size_t k = 0;
	while (len >= 4) {
		k = *(size_t *)data;

		k *= magic;
		k ^= k >> r;
		k *= magic;

		hash *= magic;
		hash ^= k;
		data += 4;
		len -= 4;
	}
	switch (len) {
		case 3: hash ^= data[2] << 16;
		case 2: hash ^= data[1] << 8;
		case 1: hash ^= data[0];
						hash *= magic;
	};
	hash ^= hash >> 13;
	hash *= magic;
	hash ^= hash >> 15;
	return hash;
}
#define MIX(h,k,m) { k *= m; k ^= k >> r; k *= m; h *= m; h ^= k; }
size_t silly_murmurHashAligned2(const uint8_t * data, int len, size_t seed) {
	const size_t magic = 0x5bd1e995;
	const int r = 24;
	size_t hash = seed ^ len;
	int align = (int)data & 3;
	if (align && (len >= 4)) {
		size_t t = 0, d = 0;
		switch (align) {
			case 1: t |= data[2] << 16;
			case 2: t |= data[1] << 8;
			case 3: t |= data[0];
		}

		t <<= (align << 3); // Same as (align * 8)

		data += 4 - align;
		len -= 4 - align;

		int sl = (4 - align) << 3;
		int sr = align << 8;

		while (len >= 4) {
			d = *(size_t *)data;
			t = (t >> sr) | (d << sl);

			size_t k = t;
			MIX(hash, k, magic);

			t = d;
			data += 4;
			len -= 4;
		}
		d = 0;
		if (len >= align) {
			switch (align) {
				case 3: d |= data[2] << 16;
				case 2: d |= data[1] << 8;
				case 1: d |= data[0];
			}

			size_t k = (t >> sr) | (d << sl);
			MIX(hash, k, magic);

			data += align;
			len -= align;

			switch (len) {
				case 3: hash ^= data[2] << 16;
				case 2: hash ^= data[1] << 8;
				case 1: hash ^= data[0];
								hash *= magic;
		};
	} else {
		switch (len) {
			case 3: d |= data[2] << 16;
			case 2: d |= data[1] << 8;
			case 1: d |= data[0];
			case 0: hash ^= (t >> sr) | (d << sl);
							hash *= magic;
		}
	}

		hash ^= hash >> 13;
		hash *= magic;
		hash ^= hash >> 15;
		return hash;
	} else {
		while (len >= 4) {
			size_t k = *(size_t *)data;
			MIX(hash, k, magic);

			data += 4;
			len -= 4;
		}

		switch (len) {
			case 3: hash ^= data[2] << 16;
			case 2: hash ^= data[1] << 8;
			case 1: hash ^= data[0];
							hash *= magic;
		};

		hash ^= hash >> 13;
		hash *= magic;
		hash ^= hash >> 15;

		return hash;
	}
}
#undef MIX
