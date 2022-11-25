//
// Created by rhys on 23/11/22.
//

#include "Memory/Heap.h"
#include "Memory/Paging.h"

Heap* Heap::kheap = NULL;

int8 heapHeaderComparator(HeapHeader* a, HeapHeader* b){
    return a->size < b->size;
}

Heap::Heap(uint32 start, uint32 end, uint32 max, uint8 supervisor, uint8 readOnly)
    : start(start), end(end), max(max), supervisor(supervisor), readOnly(readOnly),
    index((void*) start, HEAP_INDEX_SIZE, heapHeaderComparator) {

    start += sizeof(void*)*HEAP_INDEX_SIZE;

    if(start & 0xFFFFF000 == 0){
        start &= 0xFFFFF000; // Page alignment
        start += PAGE_SIZE;
    }

    this->start = start;
    this->end = end;
    this->max = max;
    this->supervisor = supervisor;
    this->readOnly = readOnly;

    HeapHeader* hole = (HeapHeader*) start;
    hole->size = end-start;
    hole->magic = HEAP_MAGIC;
    hole->isHole = TRUE;

    index.insert(hole);
}

void *Heap::alloc(uint32 size, uint8 align) {
    uint32 newSize = size + sizeof(HeapHeader) + sizeof(HeapFooter);

    int32 i = smallestHole(newSize, align);

    if(i == -1) { // No suitable hole to put heap into
        uint32 oldLength = end-start;
        uint32 oldEndAddr = end;

        expand(oldLength+newSize);

        uint32 newLength = end-start;

        i=0;
        uint32 value = 0;
        uint32 idx = -1;
        while(i<index.length()){
            uint32 tmp = (uint32) index.get(i);
            if(tmp > value){
                value = tmp;
                idx = i;
            }

            i++;
        }

        if(idx == -1){ // No headers found.
            HeapHeader* header = (HeapHeader*) oldEndAddr;
            header->magic = HEAP_MAGIC;
            header->size = newLength-oldLength;
            header->isHole = TRUE;

            HeapFooter* footer = (HeapFooter*) (oldEndAddr+header->size-sizeof(HeapFooter));
            footer->magic = HEAP_MAGIC;
            footer->header = header;

            index.insert(header);
        }else{
            HeapHeader* header = index.get(idx);
            header->size += newLength-oldLength;

            HeapFooter* footer = (HeapFooter*) ((uint32) header + header->size-sizeof(HeapFooter));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }

        return alloc(size, align);
    }

    HeapHeader* originalHeader = index.get(i);
    uint32 originalHolePos = (uint32) originalHeader;
    uint32 originalHoleSize = originalHeader->size;

    if(originalHoleSize-newSize < sizeof(HeapHeader) + sizeof(HeapFooter)){
        size += originalHoleSize-newSize;
        newSize = originalHoleSize;
    }

    if(align && originalHolePos&0xFFFFF000){
        uint32 newLoc = originalHolePos + PAGE_SIZE - (originalHolePos&0xFFF) - sizeof(HeapHeader);
        HeapHeader* holeHeader = (HeapHeader*) originalHolePos;
        holeHeader->size = PAGE_SIZE - (originalHolePos&0xFFF) - sizeof(HeapHeader);
        holeHeader->magic = HEAP_MAGIC;
        holeHeader->isHole = TRUE;

        HeapFooter* holeFooter = (HeapFooter*) (newLoc - sizeof(HeapFooter));
        holeFooter->magic = HEAP_MAGIC;
        holeFooter->header = holeHeader;
        originalHolePos = newLoc;
        originalHoleSize = originalHoleSize - holeHeader->size;
    } else {
        index.remove(i);
    }

    HeapHeader* blockHeader = (HeapHeader*) originalHolePos;
    blockHeader->magic = HEAP_MAGIC;
    blockHeader->isHole = FALSE;
    blockHeader->size = newSize;

    HeapFooter* blockFooter = (HeapFooter*) (originalHolePos+sizeof(HeapHeader)+size);
    blockFooter->magic = HEAP_MAGIC;
    blockFooter->header = blockHeader;

    if(originalHoleSize-newSize > 0){
        HeapHeader* holeHeader = (HeapHeader*) (originalHolePos+sizeof(HeapHeader) + size + sizeof(HeapFooter));
        holeHeader->magic = HEAP_MAGIC;
        holeHeader->isHole = TRUE;
        holeHeader->size = originalHoleSize - newSize;

        HeapFooter* holeFooter = (HeapFooter*) ((uint32) holeHeader + originalHoleSize - newSize - sizeof(HeapFooter));

        if((uint32) holeFooter < end){
            holeFooter->magic = HEAP_MAGIC;
            holeFooter->header = holeHeader;
        }

        index.insert(holeHeader);
    }

    return (void*) ((uint32) blockHeader + sizeof(HeapHeader));
}

void Heap::free(void* p) {
// Exit gracefully for null pointers.
    if (p == NULL)
        return;

    // Get the header and footer associated with this pointer.
    HeapHeader *header = (HeapHeader*) ( (uint32)p - sizeof(HeapHeader) );
    HeapFooter *footer = (HeapFooter*) ( (uint32)header + header->size - sizeof(HeapFooter) );

    // Make us a hole.
    header->isHole = 1;

    // Do we want to add this header into the 'free holes' index?
    char do_add = 1;

    // Unify left
    // If the thing immediately to the left of us is a footer...
    HeapFooter *test_footer = (HeapFooter*) ( (uint32)header - sizeof(HeapFooter) );
    if (test_footer->magic == HEAP_MAGIC &&
        test_footer->header->isHole == 1)
    {
        uint32 cache_size = header->size; // Cache our current size.
        header = test_footer->header;     // Rewrite our header with the new one.
        footer->header = header;          // Rewrite our footer to point to the new header.
        header->size += cache_size;       // Change the size.
        do_add = 0;                       // Since this header is already in the index, we don't want to add it again.
    }

    // Unify right
    // If the thing immediately to the right of us is a header...
    HeapHeader *test_header = (HeapHeader*) ( (uint32)footer + sizeof(HeapFooter) );
    if (test_header->magic == HEAP_MAGIC &&
        test_header->isHole)
    {
        header->size += test_header->size; // Increase our size.
        test_footer = (HeapFooter*) ( (uint32)test_header + // Rewrite it's footer to point to our header.
                                    test_header->size - sizeof(HeapFooter) );
        footer = test_footer;
        // Find and remove this header from the index.
        uint32 iterator = 0;
        while ( (iterator < index.length()) &&
                (index.get(iterator) != (void*)test_header) )
            iterator++;


        // Remove it.
        index.remove(iterator);
    }

    // If the footer location is the end address, we can contract.
    if ( (uint32)footer+sizeof(HeapFooter) == end)
    {
        uint32 old_length = end-start;
        uint32 new_length = contract( (uint32)header - start);
        // Check how big we will be after resizing.
        if (header->size - (old_length-new_length) > 0)
        {
            // We will still exist, so resize us.
            header->size -= old_length-new_length;
            footer = (HeapFooter*) ( (uint32)header + header->size - sizeof(HeapFooter) );
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        else
        {
            // We will no longer exist :(. Remove us from the index.
            uint32 iterator = 0;
            while ( (iterator < index.length()) &&
                    (index.get(iterator) != (void*)test_header) )
                iterator++;
            // If we didn't find ourselves, we have nothing to remove.
            if (iterator < index.length())
                index.remove(iterator);
        }
    }

    // If required, add us to the index.
    if (do_add == 1)
        index.insert(header);
}

int32 Heap::smallestHole(uint32 size, uint8 align) {
    uint32 i = 0;

    while(i<index.length()){
        HeapHeader* head = index.get(i);

        if(align){
            uint32 loc = (uint32) head;
            int32 offset = 0;

            if ((loc+sizeof(HeapHeader)) & 0xFFFFF000 != 0) {
                offset = PAGE_SIZE /* page size */  - (loc + sizeof(HeapHeader)) % PAGE_SIZE;
            }

            int32 holeSize = head->size-offset;

            if(holeSize >= (int32) size){
                break;
            }
        }else if(head->size >= size){
            break;
        }

        i++;
    }

    if(i == index.length()){
        return -1;
    }

    return (int32) i;
}

void Heap::expand(uint32 newSize) {
    if(newSize&0xFFFFF000 != 0){
        newSize &= 0xFFFFF000;
        newSize += PAGE_SIZE;
    }

    uint32 oldSize = end-start;

    for(int i=oldSize ; i<newSize ; i+= PAGE_SIZE){
        PageFrame::alloc(
                Paging::getPage(start+i, 1, Paging::getKernelDirectory()),
                (bool) supervisor,
                ! (bool) readOnly
                );
    }

    end = start+newSize;
}

uint32 Heap::contract(uint32 newSize) {
    if(newSize&PAGE_SIZE != 0){
        newSize &= PAGE_SIZE;
        newSize += PAGE_SIZE;
    }

    if(newSize < HEAP_MIN_SIZE){
        newSize = HEAP_MIN_SIZE;
    }

    uint32 oldSize = end-start;

    for(int i=oldSize-PAGE_SIZE ; newSize<i ; i-= PAGE_SIZE){
        PageFrame::free(
                Paging::getPage(start+i, 0, Paging::getKernelDirectory())
        );
    }

    end = start+newSize;

    return newSize;
}

Heap* Heap::getKheap() {
    return kheap;
}

void Heap::setKheap(Heap *newKheap) {
    kheap = newKheap;
}
