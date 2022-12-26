/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

/*
 * File - malloc.c Created 09.03.2019 Last Modified 30.05.2020
 */

#ifndef _CGEN_H
#define MINI
#include "cgen.h"

void free(void *p);

#endif

#ifdef CPM
static uint8_t *freeList; /* wB024 */

#define Len(p)     (uint16_t)((p)[-1] & 0x80 ? *(uint16_t *)((p)-3) : (p)[-1])
/* get the real start of the reserved block */
#define BlkAddr(p) ((p)[-1] & 0x80 ? ((p)-3) : (p)-1)
#define Link(p)    (*(uint8_t **)(p))
/*
 * the memory allocator reserves multiple blocks of memory
 * each block is stored as
 * [block size][data]
 * next points to the next allocated memory block
 * block size is encoded as 0-127 for data lengths 1-128
 * other wise (length - 3) as a word and a flag byte of 0x80
 *
 * When freed the data is replaced by a pointer to the next largest
 * free block.
 * The above implies that a minimum data length is sizeof(uint8_t *)
 *
 */
/* encode the data block size. Return the pointer to the data
 * note the passed in size should account for the block size header
 * and the minimum data length
 */
static uint8_t *setSize(register uint8_t *p, short size) {
    if (size > 0x80) {
        *((uint16_t *)p) = size - 3;
        p += 3;
        p[-1] = 0x80;
    } else {
        *p = (uint8_t)size - 1;
        p++;
    }
    return p;
}

static bool pack() {
    uint8_t *loblk;
    uint8_t *hiblkAddr;
    uint8_t **curLo;
    uint8_t **curHi;
    bool packed;
    register uint8_t *hiblk;

    packed = false;
rescan:
    for (curHi = &freeList; hiblk = *curHi; curHi = &Link(hiblk)) {
        hiblkAddr = BlkAddr(hiblk); /* optimised over original */
        for (curLo = &freeList; loblk = *curLo; curLo = &Link(loblk)) {
            if (hiblkAddr != loblk + Len(loblk)) /* loop until we find adjacent blocks */
                continue;
            /* unlink the blocks to be joined */
            if (&Link(hiblk) == curLo)      /* free list order is hi lo next */
                *curHi = Link(loblk);       /* move the next info to the hi block */
            else if (curHi == &Link(loblk)) /* free list order is low hi next */
                *curLo = Link(hiblk);       /* move the next info to the lo block */
            else {                          /* not adjacent in freelist */
                *curLo = Link(loblk);       /* move the lo -> next info */
                *curHi = Link(hiblk);       /* move the hi -> next info */
            }
            loblk = BlkAddr(loblk); /* make sure we also include the header */
            free(setSize(loblk, hiblk + Len(hiblk) - loblk)); /* join the blocks */
            packed = true;
            goto rescan;
        }
    }
    /* Bug? the original code unintentionally returned int(curHi) + 1 */
    return packed;
}

/*********************************************************
 * malloc v2 OK				Used in: allocMem
 *********************************************************/
void *malloc(size_t size) {
    uint8_t *l1;
    size_t l2;
    bool done;
    register uint8_t *sst;

    if (size < sizeof(uint8_t *))
        size = sizeof(uint8_t *);
    done = false;
    /*
     * see if we can find a suitable previously allocated block
     * if not consolidate and try again
     * failing that allocate a new block
     */
    do {
        for (sst = (uint8_t *)&freeList; l1 = Link(sst); sst = l1) {
            if (Len(l1) >= size) {
                Link(sst) = Link(l1);
                sst       = l1;
                /* if splitting the block would leave sufficient space
                 * for a new block i.e. header + data/pointer then split it */
                if ((l2 = Len(sst)) > size + 2 + sizeof(uint8_t *)) { /* ? +1 ok */
                    l1 = sst + size;
                    free(setSize(l1, l2 - size));
                    sst = BlkAddr(sst);
                    return setSize(sst, size);
                } else
                    return sst;
            }
        }
    } while (!done && (done = pack()));
    l2 = size < 128 ? 1 : 3; /* size of block header */
    if ((sst = sbrk(size + l2)) == (uint8_t *)-1)
        return 0;

    return setSize(sst, size + l2);
}

/*********************************************************
 * free v6 OK  Used in: leaveBlock,  sub_36E0, releaseNodeFreeList
 *			      sub_5DF6, sub_6D61, cmalloc
 * free the given block inserting on free chain in size order
 *
 *********************************************************/
void free(void *p) {
    register uint8_t *st;

    for (st = (uint8_t *)&freeList; Link(st) && Len(Link(st)) < Len((uint8_t *)p); st = Link(st))
        ;
    Link(p)  = Link(st);
    Link(st) = (uint8_t *)p;
}
#endif
/* end of file malloc.c */
