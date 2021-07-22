///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2021 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission Fall 2020, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Main File:        myHeap.c
// This File:        myHeap.c
// Other Files:      myHeap.h
// Semester:         CS 354 Spring 2021
// Instructor:       deppeler
// 
// Author:           Shreyans Sakhlecha
// Email:            sakhlecha@wisc.edu
// CS Login:         sakhlecha
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//                   
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////
// DEB'S PARTIAL SOLUTION FOR SPRING 2021 DO NOT SHARE

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "myHeap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

	int size_status;
	/*
	 * Size of the block is always a multiple of 8.
	 * Size is stored in all block headers and in free block footers.
	 *
	 * Status is stored only in headers using the two least significant bits.
	 *   Bit0 => least significant bit, last bit
	 *   Bit0 == 0 => free block
	 *   Bit0 == 1 => allocated block
	 *
	 *   Bit1 => second last bit 
	 *   Bit1 == 0 => previous block is free
	 *   Bit1 == 1 => previous block is allocated
	 * 
	 * End Mark: 
	 *  The end of the available memory is indicated using a size_status of 1.
	 * 
	 * Examples:
	 * 
	 * 1. Allocated block of size 24 bytes:
	 *    Allocated Block Header:
	 *      If the previous block is free      p-bit=0 size_status would be 25
	 *      If the previous block is allocated p-bit=1 size_status would be 27
	 * 
	 * 2. Free block of size 24 bytes:
	 *    Free Block Header:
	 *      If the previous block is free      p-bit=0 size_status would be 24
	 *      If the previous block is allocated p-bit=1 size_status would be 26
	 *    Free Block Footer:
	 *      size_status should be 24
	 */
} blockHeader;         

/* Global variable - DO NOT CHANGE. It should always point to the first block,
 * i.e., the block at the lowest address.
 */
blockHeader *heapStart = NULL;     

/* Size of heap allocation padded to round to nearest page size.
*/
int allocsize;

/*
 * Additional global variables may be added as needed below
 */


/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if not positive or if larger than heap space.
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 * - If a BEST-FIT block found is NOT found, return NULL
 *   Return NULL unable to find and allocate block for desired size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* myAlloc(int size) {     
	//TODO: Your code goes in here.
	if((size < 1) || (size > allocsize)){ //checks for valid size.
		return NULL;
	}
	int hdr = size +4;//add 4 for hdr
	int align = 0;//will store the block size with padding and header.
	if ((hdr % 8) == 0){
		align = hdr;
	}
	else{
		int pad = 8 - (hdr %8);
		align = hdr + pad;
	}
	blockHeader *current = heapStart; //blockHeader to traverse through the heap.
	blockHeader *best = heapStart;// blockHeader to store the best-fit found.
	blockHeader *next = current; // blockHeader to change p-bit of the next block.
	int found = -1; //indicates if best-fit found.
	while(best -> size_status != 1){
		int ss = best -> size_status; //extract size and status of a block header
		int a = -1;//a-bit
		int block = -1;//block size

		//determining p-bit, a-bit and block size.
		if((ss%8) == 0) { 
			a = 0;
			block = ss;
		}
		else if ((ss%8) == 1){
			a = 1;
			block = ss - 1;
		}
		else if ((ss%8) == 2){
			a = 0;
			block = ss -2;
		}
		else if ((ss%8) == 3){
			a = 1;
			block = ss - 3;
		}
		if (a == 1){ //if the block is alloc'd go to the next block.
			best  = (blockHeader*)((char*)best + block);
			continue;
		}
		else if (a == 0){//if the block is free.
			if (block < align){//check block size.
				best = (blockHeader*)((char*)best + block);

			}
			else if (block >= align){//best-fit found.
				found = 1;
				break;
			}

		}
		
	}
	if (found == -1){//if no block is found.
		return NULL;
	}

	while(current -> size_status != 1){//travese thought the heap.
		int ss = current -> size_status;//similar as previous while loop.
		int a = -1;
		int block = -1;

		if((ss%8) == 0){
			a = 0;
			block = ss;
		}
		else if ((ss%8) == 1){
			a = 1;
			block = ss - 1;
		}
		else if ((ss%8) == 2){
			a = 0;
			block = ss -2;
		}
		else if ((ss%8) == 3){
			a = 1;
			block = ss - 3;
		} 
		if (a == 1){
			current  = (blockHeader*)((char*)current + block);
			continue;
		}
		else if (a == 0){
			if(block == align){//exact match is found.
				int h = current -> size_status;
				current -> size_status = h + 1;//set a-bit of the block to 1.
				next = (blockHeader*)((char*)current + block);
				if (next -> size_status != 1){//set p-bit of next to 1.
					int x = next->size_status;
					next -> size_status = x +2;
				}
				return (void*)((char*)current + 4);//return payload address.
			}
			else if ( block < align){//block size is smaller than the requested size.
				current  = (blockHeader*)((char*)current + block);
			}
			else if (block > align){//block size is bigger.
				int b = best->size_status;
				b = b - (b%8);
				if (block < b){//smaller than best-fit.
					best = current;
				}
				if (block >= b){//bigger than best fit.
					current = (blockHeader*)((char*)current + block);
				}
			}
		}

	}
	int y = best -> size_status;//extract size and status of best-fit.
	int w = y;
	y = y - (y%8);//size of best fit.
	if ((w%8) == 0 || (w%8) == 2){//checks if a-bit is 0.
		if (y > align){
			int fre = y - align;
			blockHeader *split = (blockHeader*) ((char*)best + align);
			split -> size_status = fre + 2;//p-bit = 1, a-bit = 0.
			best -> size_status = best-> size_status - fre + 1; //so that p-bit remains unchanged while a-bit changes from 0 to 1.
			return (void*)((char*)best + 4);//returns payload addr.
		}
		if (y < align){
			return NULL;
		}
	}


	return NULL;
} 

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int myFree(void *ptr) {    
	//TODO: Your code goes in here.
	if (ptr == NULL){//checks that passed address is not NULL.
		return -1;
	}
	if (((int)ptr % 8) != 0){//checks that the given addr is a multiple of 8. 
		return -1;
	}
	blockHeader* end = (blockHeader*)((char*)heapStart + allocsize);
	if ((int*)ptr < (int*)heapStart || (int*)ptr > (int*)end){ //chcks if the pointer is a part of the alloted heap.
		return -1;
	}
	blockHeader* current = (blockHeader*)((char*)ptr - 4);//skip to next block.
	int ss = current -> size_status;//similar as myalloc func.
	int a = -1;
	int block = -1;

	if((ss%8) == 0){
		a = 0;
		block = ss;
	}
	else if ((ss%8) == 1){
		a = 1;
		block = ss - 1;
	}
	else if ((ss%8) == 2){
		a = 0;
		block = ss -2;
	}
	else if ((ss%8) == 3){
		a = 1;
		block = ss - 3;
	}
	if (a == 0){//if block is already free.
		return -1;
	}

	else if (a == 1){
		ss = ss -1; //setting a-bit to 0.
		current->size_status = ss;
		blockHeader* next;//ptr to next block.
		next = (blockHeader*)((char*)current + block);
		if (next -> size_status != 1){
			int x = next->size_status;
			next -> size_status = x -2; //set p-bit of next block to 0.
		}
		return 0;
	}
	return -1;
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for delayed coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
	//TODO: Your code goes in here.
	blockHeader* current = heapStart;//ptr to traverse the heap.
	blockHeader *next = heapStart;//ptr to next block.
	while ( (current->size_status != 1) && (next ->size_status != 1)){//till current and next != end mark.
		int ss = current -> size_status;
		int a = -1;
		int block = -1;

		if((ss%8) == 0){
			a = 0;
			block = ss;
		}
		else if ((ss%8) == 1){
			a = 1;
			block = ss - 1;
		}
		else if ((ss%8) == 2){
			a = 0;
			block = ss -2;
		}
		else if ((ss%8) == 3){
			a = 1;
			block = ss - 3;
		}
		if (a == 1){//if the block is alloc'd
			current = (blockHeader*)((char*)current + block);
			continue;
		}
		else if (a == 0){//if block is free.
			next = (blockHeader*)((char*)current + block);
			int ss1 = next -> size_status;
			int a1 = -1;
			int block1 = -1;
			if (ss1 == 1){//if the next block is the end mark.
				return 0;
			}

			if((ss1%8) == 0){
				a1 = 0;
				block1 = ss1;
			}
			else if ((ss1%8) == 1){
				a1 = 1;
				block1 = ss1 - 1;
			}
			else if ((ss1%8) == 2){
				a1 = 0;
				block1 = ss1 -2;
			}
			else if ((ss1%8) == 3){
				a1 = 1;
				block1 = ss1 - 3;
			}
			if(a1 == 1){//if the adjacent block is alloc'd 
				current = (blockHeader*)((char*)current + block);
				continue;
			}
			else if (a1 == 0){//if the adjacent block is free.
				current -> size_status = ss + block1;//coalesce the two blocks.
			}
		}
	}

	return 0;
}


/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int myInit(int sizeOfRegion) {    

	static int allocated_once = 0; //prevent multiple myInit calls

	int pagesize;   // page size
	int padsize;    // size of padding when heap size not a multiple of page size
	void* mmap_ptr; // pointer to memory mapped area
	int fd;

	blockHeader* endMark;

	if (0 != allocated_once) {
		fprintf(stderr, 
				"Error:mem.c: InitHeap has allocated space during a previous call\n");
		return -1;
	}

	if (sizeOfRegion <= 0) {
		fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
		return -1;
	}

	// Get the pagesize
	pagesize = getpagesize();

	// Calculate padsize as the padding required to round up sizeOfRegion 
	// to a multiple of pagesize
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;

	allocsize = sizeOfRegion + padsize;

	// Using mmap to allocate memory
	fd = open("/dev/zero", O_RDWR);
	if (-1 == fd) {
		fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
		return -1;
	}
	mmap_ptr = mmap(NULL, allocsize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (MAP_FAILED == mmap_ptr) {
		fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
		allocated_once = 0;
		return -1;
	}

	allocated_once = 1;

	// for double word alignment and end mark
	allocsize -= 8;

	// Initially there is only one big free block in the heap.
	// Skip first 4 bytes for double word alignment requirement.
	heapStart = (blockHeader*) mmap_ptr + 1;

	// Set the end mark
	endMark = (blockHeader*)((void*)heapStart + allocsize);
	endMark->size_status = 1;

	// Set size in header
	heapStart->size_status = allocsize;

	// Set p-bit as allocated in header
	// note a-bit left at 0 for free
	heapStart->size_status += 2;

	// Set the footer
	blockHeader *footer = (blockHeader*) ((void*)heapStart + allocsize - 4);
	footer->size_status = allocsize;

	return 0;
} 

/* 
 * Function to be used for DEBUGGING to help you visualize your heap structure.
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void dispMem() {     

	int counter;
	char status[6];
	char p_status[6];
	char *t_begin = NULL;
	char *t_end   = NULL;
	int t_size;

	blockHeader *current = heapStart;
	counter = 1;

	int used_size = 0;
	int free_size = 0;
	int is_used   = -1;

	fprintf(stdout, 
			"*********************************** Block List **********************************\n");
	fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
	fprintf(stdout, 
			"---------------------------------------------------------------------------------\n");

	while (current->size_status != 1) {
		t_begin = (char*)current;
		t_size = current->size_status;

		if (t_size & 1) {
			// LSB = 1 => used block
			strcpy(status, "alloc");
			is_used = 1;
			t_size = t_size - 1;
		} else {
			strcpy(status, "FREE ");
			is_used = 0;
		}

		if (t_size & 2) {
			strcpy(p_status, "alloc");
			t_size = t_size - 2;
		} else {
			strcpy(p_status, "FREE ");
		}

		if (is_used) 
			used_size += t_size;
		else 
			free_size += t_size;

		t_end = t_begin + t_size - 1;

		fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
				p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

		current = (blockHeader*)((char*)current + t_size);
		counter = counter + 1;
	}

	fprintf(stdout, 
			"---------------------------------------------------------------------------------\n");
	fprintf(stdout, 
			"*********************************************************************************\n");
	fprintf(stdout, "Total used size = %4d\n", used_size);
	fprintf(stdout, "Total free size = %4d\n", free_size);
	fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
	fprintf(stdout, 
			"*********************************************************************************\n");
	fflush(stdout);

	return;  
} 


// end of myHeap.c (sp 2021)                                         

