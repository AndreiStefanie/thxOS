#include "bitmap.h"

//
//
//
void bitmap_init(bitmap_t *bm)
{
	bm->bit_cnt = BM_MAX;
	for (size_t i = 0; i < BM_SIZE; i++)
	{
		bm->map[i] = 0;
	}
}

//
//
//
void bitmap_destroy(bitmap_t *bm)
{
	bitmap_init(bm);
}

//
//
//
bool bitmap_test(bitmap_t *bm, size_t index)
{
	return bm->map[BIT_INDEX(index)] & (1ull << BIT_SHIFT(index));
}

//
//
//
void bitmap_set(bitmap_t *bm, size_t index, bool bit)
{
	if (bit)
	{
		bm->map[BIT_INDEX(index)] |= (1ull << BIT_SHIFT(index));
	}
	else
	{
		bm->map[BIT_INDEX(index)] &= ~(1ull << BIT_SHIFT(index));
	}
}

//
//
//
void bitmap_flip(bitmap_t *bm, size_t index)
{
	bitmap_set(bm, index, !bitmap_test(bm, index));
}

//
//
//
void bitmap_set_all(bitmap_t *bm, bool bit)
{
	for (size_t i = 0; i < bm->bit_cnt; i++)
	{
		bitmap_set(bm, i, bit);
	}
}

//
// Search for the first group of bits having the given
// length and value.
//
size_t bitmap_scan(bitmap_t *bm, size_t count, bool bit)
{
	size_t available_start = BM_ERROR;
	size_t remaining_cnt = bm->bit_cnt - count;

	if (count > BM_MAX || count < 1)
	{
		return BM_ERROR;
	}

	
	for (size_t i = 0; (i <= remaining_cnt) && (available_start == BM_ERROR); i++)
	{
		if (bitmap_test(bm, i) != bit)
		{
			continue;
		}

		size_t j = 1;
		for (; j < count; j++)
		{
			if (bitmap_test(bm, i + j) != bit)
			{
				break;
			}
		}
		if (j == count - 1 || count == 1)
		{
			available_start = i;
		}
	}

	return available_start;
}

//
//
//
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
