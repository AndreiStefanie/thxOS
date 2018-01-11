#pragma once
#include "..\util\bitmap.h"
#include "..\defs.h"

#define PAGE_FREE false
#define PAGE_USED true

#define PAGE_ERROR BM_ERROR

#define PAGE_SIZE        0x1000
#define PAGE_SIZE_LARGE  0x200000
#define PAGE_SIZE_HUGE   0x40000000

// Virtual address bitmasks and shifts
#define PGSHIFT_PML4E    39
#define PGSHIFT_PDPTE    30
#define PGSHIFT_PDE      21
#define PGSHIFT_PTE      12
#define PGMASK_ENTRY     0x1ff
#define PGMASK_OFFSET    0x3ff

// Virtual address subfield accessors
#define PML4E(a) (((a) >> PGSHIFT_PML4E) & PGMASK_ENTRY)
#define PDPTE(a) (((a) >> PGSHIFT_PDPTE) & PGMASK_ENTRY)
#define PDE(a) (((a) >> PGSHIFT_PDE) & PGMASK_ENTRY)
#define PTE(a) (((a) >> PGSHIFT_PTE) & PGMASK_ENTRY)

struct pte
{
    bool is_global;
    bool is_dirty;
    bool is_accessed;
    bool is_cache_disabled;
    bool is_user;
    bool is_rw;
    bool is_present;
    bool reserved[11];
    uint64 frame;
};
typedef struct pte pte_t;

struct page
{
    pte_t entry;
    size_t index;
    uint8 space[PAGE_SIZE];
};
typedef struct page page_t;

void mem_init();
page_t *mem_alloc_page();
void mem_free_page(page_t *page);
uint64 mem_get_start_address(page_t *page);
