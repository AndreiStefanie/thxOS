#pragma once
#include "defs.h"

// the underlying structure used by the bitmap
#define BM_TYPE uint64_t
#define BM_SIZE 64

// how many bits fit in the underlying data type
#define BM_CNT (sizeof (BM_TYPE) * CHAR_BIT)
#define BM_MAX BM_CNT * BM_SIZE

#define BM_ERROR SIZE_MAX

#define BIT_SHIFT(i) ((i) % BM_CNT)
#define BIT_INDEX(i) ((i) / BM_CNT)

struct bitmap
{
	size_t bit_cnt;         // Number of bits.
	BM_TYPE map[BM_SIZE];   // The actual bitmap.
};

typedef struct bitmap bitmap_t;

void bitmap_init(bitmap_t *bm);
void bitmap_destroy(bitmap_t *bm);

bool bitmap_test(bitmap_t *bm, size_t index);
void bitmap_set(bitmap_t *bm, size_t index, bool bit);
void bitmap_flip(bitmap_t *bm, size_t index);

void bitmap_set_all(bitmap_t *bm, bool bit);
size_t bitmap_scan(bitmap_t *bm, size_t count, bool bit);
size_t bitmap_scan_and_flip(bitmap_t *bm, size_t count, bool bit);
