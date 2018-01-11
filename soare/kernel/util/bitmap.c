#include "bitmap.h"

bitmap_t memory_bm;

void bitmap_init(bitmap_t *bm)
{
	bm->bit_cnt = BM_CNT;
	bm->map = 0;
}

void bitmap_destroy(bitmap_t *bm)
{
	bitmap_init(bm);
}

bool bitmap_test(bitmap_t *bm, size_t index)
{
	BM_TYPE *map = &bm->map;
	
	return (map[index / BM_CNT] >> (index % BM_CNT)) & 1ull;
}

void bitmap_set(bitmap_t *bm, size_t index, bool bit)
{
	BM_TYPE *map = &bm->map;

	map[index / BM_CNT] |= 1ull << (bit % BM_CNT);
}

void bitmap_flip(bitmap_t *bm, size_t index)
{
	bitmap_set(bm, index, !bitmap_test(bm, index));
}

void bitmap_set_all(bitmap_t *bm, bool bit)
{
	for (size_t i = 0; i < bm->bit_cnt; i++)
	{
		bitmap_set(bm, i, bit);
	}
}

size_t bitmap_scan(bitmap_t *bm, size_t count, bool bit)
{
	size_t available_start = BM_ERROR;

	if (count > BM_CNT)
	{
		return BM_ERROR;
	}

	for (size_t i = 0; (i <= bm->bit_cnt - count) && (available_start == BM_ERROR); i++)
	{
		if (bitmap_test(bm, i) != bit)
		{
			continue;
		}

		for (size_t j = 1; j < count; j++)
		{
			if (bitmap_test(bm, i) != bit)
			{
				break;
			}
			if (j == count - 1)
			{
				available_start = i;
			}
		}
	}

	return available_start;
}

size_t bitmap_scan_and_flip(bitmap_t *bm, size_t count, bool bit)
{
	size_t available_start;

	available_start = bitmap_scan(bm, count, bit);
	if (BM_ERROR == available_start)
	{
		return BM_ERROR;
	}

	for (size_t i = 0; i < count; i++)
	{
		bitmap_set(bm, available_start + i, !bit);
	}
	
	return available_start;
}
