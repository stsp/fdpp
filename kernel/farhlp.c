#include <stdlib.h>
#include "portab.h"
#include "farptr.hpp"

/* hackish helper to store/lookup far pointers - using static
 * object (map) is an ugly hack in an OOP world.
 * Need this to work around some C++ deficiencies, see comments
 * in farptr.hpp */

struct f_m {
    const void *p;
    far_t f;
};
static struct f_m *far_map;
static int f_m_size;
static int f_m_len;
#define INIT_SIZE 128

static int do_lookup(const void *ptr)
{
    int i;

    for (i = 0; i < f_m_len; i++) {
        if (far_map[i].p == ptr)
            return i;
    }
    return -1;
}

void store_far(const void *ptr, far_t fptr)
{
    int idx;
    struct f_m *fm;

    if (!f_m_size) {
        f_m_size = INIT_SIZE;
        far_map = (struct f_m *)malloc(f_m_size * sizeof(struct f_m));
        _assert(far_map);
    }
    _assert(f_m_len <= f_m_size);
    if (f_m_len == f_m_size) {
        f_m_size *= 2;
        far_map = (struct f_m *)realloc(far_map, f_m_size * sizeof(struct f_m));
        _assert(far_map);
    }
    idx = do_lookup(ptr);
    if (idx != -1) {
        far_t *f = &far_map[idx].f;
        _assert(f->seg == fptr.seg && f->off == fptr.off);
        /* already exists, do nothing */
        return;
    }
    fm = &far_map[f_m_len++];
    fm->p = ptr;
    fm->f = fptr;
}

far_t lookup_far(const void *ptr)
{
    int idx = do_lookup(ptr);
    _assert(idx != -1);
    return far_map[idx].f;
}
