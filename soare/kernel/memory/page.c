#include "page.h"
#include "..\interrupts\isr.h"

static mem_init_page(size_t i);

static bitmap_t mem_bm;
page_t page_table[BM_MAX];

//
// Initialize memory management.
//
void mem_init()
{
    bitmap_init(&mem_bm);
    register_interrupt_handler(0x0e, &pagefault_callback);

    for (size_t i = 0; i < BM_MAX; i++)
    {
        mem_init_page(i);
    }
}

//
// Allocate the first available page.
//
page_t *mem_alloc_page()
{
    size_t page_index = bitmap_scan_and_flip(&mem_bm, 1, PAGE_FREE);

    if (BM_ERROR == page_index)
    {
        return PAGE_ERROR;
    }

    page_t *page = &page_table[page_index];
    page->is_present = true;
    page->is_rw = true;
    
    return page;
}

//
// Free the given page in the page table.
//
void mem_free_page(page_t *page)
{
    mem_init_page(page->index);
    bitmap_set(mem_bm, page->index, PAGE_FREE);
}

//
// Retrieve the starting address of the allocated space.
//
uint64 mem_get_start_address(page_t *page)
{
    return &page->space[0];
}

//
// Page fault exception handler
//
static void pagefault_callback(interrupt_context_t *context)
{
    UNUSED(context);
}

//
// Initialize a page identified by its index.
//
static mem_init_page(size_t i)
{
    page_t *page = &page_table[i];

    page->entry.is_global = false;
    page->entry.is_dirty = false;
    page->entry.is_accessed = false;
    page->entry.is_cache_disabled = false;
    page->entry.is_user = false;
    page->entry.is_rw = false;
    page->entry.is_present = false;
    page->entry.frame = 0;

    page->index = i;

    for (size_t j = 0; j < PAGE_SIZE; j++)
    {
        page->space[j] = 0;
    }
}
