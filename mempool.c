#ifdef OS_WINDOWS
#	define HARBOL_LIB
#endif

#include "harbol.h"


static size_t AlignSize(const size_t size, const size_t align)
{
	return (size + (align-1)) & -align;
}

HARBOL_EXPORT void harbol_mempool_init(struct HarbolMemoryPool *const mempool, const size_t size)
{
#ifdef POOL_NO_MALLOC
	(void)size;
#endif
	if( !mempool )
		return;
	
	memset(mempool, 0, sizeof *mempool);
	
#ifdef POOL_NO_MALLOC
	// allocate the mempool bottom towards the very end of the mempool.
	mempool->HeapBottom = mempool->HeapMem + POOL_HEAPSIZE;
	mempool->HeapSize = POOL_HEAPSIZE;
#else
	// align the mempool size to at least the size of an alloc node.
	mempool->HeapSize = AlignSize(size, sizeof(struct HarbolAllocNode));
	mempool->HeapMem = calloc(mempool->HeapSize+1, sizeof *mempool->HeapMem);
	//printf("harbol_mempool_init :: mempool->HeapSize == %zu\n", mempool->HeapSize);
	if( !mempool->HeapMem )
		return;
	
	mempool->HeapBottom = mempool->HeapMem + mempool->HeapSize;
#endif
}

HARBOL_EXPORT void harbol_mempool_del(struct HarbolMemoryPool *const mempool)
{
	if( !mempool )
		return;
	
#ifndef POOL_NO_MALLOC
	if( mempool->HeapMem )
		free(mempool->HeapMem);
#endif
	memset(mempool, 0, sizeof *mempool);
}

HARBOL_EXPORT void *harbol_mempool_alloc(struct HarbolMemoryPool *const restrict mempool, const size_t size)
{
	if( !mempool || !size )
		return NULL;
	
	struct HarbolAllocNode *new_mem = NULL;
	const size_t alloc_size = size + sizeof *new_mem;
	
	// if the freelist is valid, let's allocate FROM the freelist then!
	if( mempool->FreeList ) {
		struct HarbolAllocNode **node_iter = NULL;
		for(
			// start at the freelist head.
			node_iter = &mempool->FreeList ;
			// make sure it's smaller than the allocation size.
			*node_iter && (*node_iter)->Size < alloc_size ;
			// go to the next node if it's smaller than the needed size.
			node_iter = &(*node_iter)->Next
		);
		
		// if we got here, that means we found a size that's good.
		if( *node_iter ) {
			struct HarbolAllocNode *const n = *node_iter;
			//puts("harbol_mempool_alloc :: *node_iter is valid.");
			if( (uintptr_t)n < (uintptr_t)mempool->HeapMem || ((uintptr_t)n - (uintptr_t)mempool->HeapMem) >= mempool->HeapSize || n->Size >= mempool->HeapSize || !n->Size )
				return NULL;
			
			const size_t mem_split_threshold = sizeof(intptr_t);
			if( n->Size < alloc_size + mem_split_threshold ) {
				//puts("harbol_mempool_alloc :: allocating close sized node");
				/* close in size - reduce fragmentation by not splitting */
				new_mem = *node_iter;
				*node_iter = new_mem->Next;
				new_mem->Next = NULL;
				mempool->NodeCount--;
			} else {
				/* split this big memory chunk */
				//puts("harbol_mempool_alloc :: allocating split up node");
				new_mem = (struct HarbolAllocNode *)( (uint8_t *)n + (n->Size - alloc_size) );
				//if( (uintptr_t)new_mem < (uintptr_t)mempool->HeapMem || ((uintptr_t)new_mem - (uintptr_t)mempool->HeapMem) >= mempool->HeapSize )
				//	return NULL;
				
				n->Size -= alloc_size;
				new_mem->Size = alloc_size;
				new_mem->Next = NULL;
			}
		}
	}
	if( !new_mem ) {
		//puts("harbol_mempool_alloc :: allocating from main mempool.");
		// couldn't allocate from a freelist
		if( mempool->HeapBottom-alloc_size < mempool->HeapMem )
			return NULL;
		
		// allocate from available mempool.
		// subtract allocation size from the mempool.
		mempool->HeapBottom -= alloc_size;
		// use the available mempool space as the new node.
		new_mem = (struct HarbolAllocNode *)mempool->HeapBottom;
		new_mem->Size = alloc_size;
		new_mem->Next = NULL;
	}
	// here's the structure of the allocation block.
	// --------------
	// | mem size   | lowest addr of block
	// | next node  |
	// --------------
	// | allocated  |
	// |   memory   |
	// |   space    | highest addr of block
	// --------------
	void *restrict ret_mem = (uint8_t *)new_mem + sizeof *new_mem;
	memset(ret_mem, 0, new_mem->Size - sizeof *new_mem);
	//printf("harbol_mempool_alloc :: alloc_size - sizeof *new_mem == %zu\n", alloc_size - sizeof *new_mem);
	return ret_mem;
}

HARBOL_EXPORT void *harbol_mempool_realloc(struct HarbolMemoryPool *const restrict mempool, void *ptr, const size_t newsize)
{
	if( !ptr ) // NULL ptr should make this work like regular Allocation.
		return harbol_mempool_alloc(mempool, newsize);
	else if( (uintptr_t)ptr <= (uintptr_t)mempool->HeapMem )
		return NULL;
	
	struct HarbolAllocNode *ptr_node = (struct HarbolAllocNode *)((char *)ptr - sizeof *ptr_node);
	const size_t node_size = sizeof *ptr_node;
	
	char *resized_block = harbol_mempool_alloc(mempool, newsize);
	if( !resized_block )
		return NULL;
	
	struct HarbolAllocNode *resized_node = (struct HarbolAllocNode *)(resized_block - sizeof *resized_node);
	memmove(resized_block, ptr, ptr_node->Size > resized_node->Size ? (resized_node->Size - node_size) : (ptr_node->Size - node_size));
	harbol_mempool_dealloc(mempool, ptr);
	return resized_block;
}

HARBOL_EXPORT void harbol_mempool_dealloc(struct HarbolMemoryPool *const restrict mempool, void *ptr)
{
	if( !mempool || !ptr || (uintptr_t)ptr <= (uintptr_t)mempool->HeapMem )
		return;
	
	// behind the actual pointer data is the allocation info.
	struct HarbolAllocNode *mem_node = (struct HarbolAllocNode *) ((char *)ptr - sizeof *mem_node);
	if( !mem_node->Size || (mem_node->Size > mempool->HeapSize) || (((uintptr_t)ptr - (uintptr_t)mempool->HeapMem) > mempool->HeapSize) )
		return;
	
	//memset(ptr, 0, mem_node->Size - sizeof *mem_node);
	if( (uintptr_t)mem_node == (uintptr_t)mempool->HeapBottom ) {
		mempool->HeapBottom += mem_node->Size;
	}
	else if( !mempool->FreeList || ( (uintptr_t)mempool->FreeList >= (uintptr_t)mempool->HeapMem && (uintptr_t)mempool->FreeList - (uintptr_t)mempool->HeapMem < mempool->HeapSize ) ) {
		/* put it in the big memory freelist */
		// first check if we already have the pointer in the freelist.
		for( struct HarbolAllocNode *n=mempool->FreeList; n; n=n->Next )
			// if mem_node's data is part of the freelist, return as we've already freed it.
			if( (uintptr_t)n == (uintptr_t)mem_node )
				return;
		
		mem_node->Next = mempool->FreeList;
		mempool->FreeList = mem_node;
		mempool->NodeCount++;
		if( mempool->NodeCount>10 )
			harbol_mempool_defrag(mempool);
	}
}

HARBOL_EXPORT void harbol_mempool_destroy(struct HarbolMemoryPool *const restrict mempool, void *ptr)
{
	if( !mempool || !ptr )
		return;
	
	void **const restrict ptrref = ptr;
	harbol_mempool_dealloc(mempool, *ptrref), *ptrref=NULL;
}

HARBOL_EXPORT size_t harbol_mempool_get_remaining(const struct HarbolMemoryPool *const mempool)
{
	if( !mempool || !mempool->HeapMem )
		return 0;
	size_t total_remaining = (uintptr_t)mempool->HeapBottom - (uintptr_t)mempool->HeapMem;
	if( mempool->FreeList )
		for( struct HarbolAllocNode *n=mempool->FreeList; n; n=n->Next )
			total_remaining += n->Size;
	return total_remaining;
}

HARBOL_EXPORT size_t harbol_mempool_get_heap_size(const struct HarbolMemoryPool *const mempool)
{
	return !mempool || !mempool->HeapMem ? 0 : mempool->HeapSize;
}

HARBOL_EXPORT struct HarbolAllocNode *harbol_mempool_get_freelist(const struct HarbolMemoryPool *const mempool)
{
	return !mempool ? NULL : mempool->FreeList;
}

HARBOL_EXPORT bool harbol_mempool_defrag(struct HarbolMemoryPool *const mempool)
{
	if( !mempool || !mempool->FreeList )
		return false;
	///*
	// if the memory pool has been entirely released, fully defrag it.
	if( mempool->HeapSize == harbol_mempool_get_remaining(mempool) ) {
		mempool->FreeList = NULL;
		memset(mempool->HeapMem, 0, mempool->HeapSize);
		mempool->HeapBottom = mempool->HeapMem + mempool->HeapSize;
		mempool->NodeCount = 0;
		return true;
	}
	//*/
	// Check if the nodes are close to the mempool's bottom "stack".
	// if it is, pop the Allocation Block && increase the mempool bottom.
	struct HarbolAllocNode
		**iter = &mempool->FreeList,
		*restrict prev = NULL,
		*next = NULL
	;
	while( *iter ) {
		struct HarbolAllocNode *curr = *iter;
		next = curr->Next;
		
		// Patch: last node's sizing gets zeroed somehow but the node is never removed.
		if( curr && !next && !curr->Size ) {
			prev->Next = *iter = NULL;
			mempool->NodeCount--;
			break;
		}
		
		if( (uintptr_t)curr == (uintptr_t)mempool->HeapBottom ) {
			*iter = next;
			mempool->HeapBottom += curr->Size;
			mempool->NodeCount--;
			continue;
		}
		else if( (uintptr_t)curr + curr->Size==(uintptr_t)next ) {
			curr->Size += next->Size;
			curr->Next = next->Next;
			mempool->NodeCount--;
			continue;
		}
		
		/*
			prev - X > [size:16 | next:A | memory block]
			next - B > [size:12 | next:XYZ | memory block]
			curr - A > [size:8 | next:B | memory block]
			
			issue: we need to know the node previous to A...
			Add B's size with A.
			set the prev node's Next ptr to point to B.
			set the curr node to B
			
			result:
			prev - X > [size:16 | next:B | memory block]
			curr - B > [size:20 | next:XYZ | memory block]
		*/
		else if( prev && next && (uintptr_t)curr - next->Size==(uintptr_t)next ) {
			next->Size += curr->Size;
			prev->Next = next;
			*iter = next;
			mempool->NodeCount--;
			continue;
		}
		prev = curr;
		iter = &curr->Next;
	}
	/*
	for( struct HarbolAllocNode *restrict n=mempool->FreeList; n; n = n->Next )
		printf(!n->Size ? "n == %zu | next == %zu | size of 0.\n" : "n == %zu | next == %zu\n", (uintptr_t)n, (uintptr_t)n->Next);
	printf("HeapBottom == %zu\n", (uintptr_t)mempool->HeapBottom);
	*/
	return true;
}
