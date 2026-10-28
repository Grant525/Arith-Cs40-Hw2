/***************************************************************
 *
 *                     uarray2b.c
 *
 *        Assignment: CS 40 HW 3, Locality
 *        Authors:    Micheal Gonzales (mgonza20),
 *                    Grant Caile (gcaile01)
 *        Date:       October 14, 2026
 *
 *        Implementation of the blocked 2D array interface in
 *        uarray2b.h. The array is divided into square blocks of
 *        blocksize x blocksize cells, and the cells of each block
 *        are stored contiguously in memory, so that cells near each
 *        other in the image are usually near each other in memory.
 *        UArray2b_map visits every cell in one block before moving
 *        to the next, which gives good cache locality.
 *
 *        Clients should include uarray2b.h. This implementation
 *        uses UArray2 (uarray2.h) to hold the blocks and Hanson's
 *        UArray (uarray.h) to hold the cells of each block.
 * 
 *        The interface provides functions to:
 *        - create and free a 2D array
 *        - get its width, height, element size, and blocksize
 *        - get a pointer to the element at a given column and row
 *        - apply a function to every element, in either row-major
 *        or column-major order
 *
 *        Raises a checked runtime error if a dimension or the element
 *        size is negative, if memory allocation fails, if a NULL array
 *        is passed in, or if an index is out of bounds.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <iso646.h>
#include <assert.h>
#include <math.h>

#include "uarray.h"
#include "except.h"
#include "uarray2.h"
#include "uarray2b.h"

Except_T Memory_Error_b = { "Memory allocation failed" };
Except_T Bound_Error_b = { "Out of bounds reference" };
Except_T Args_Error_b = { "Unexpected arguments" };


struct UArray2b_T {
        int width; 
        int height;
        int size;
        int blocksize;
        UArray2_T elems;
};

#define T UArray2b_T

/********** UArray2b_new ********
 *
 * Creates a new blocked 2D array
 *
 * Parameters:
 *      int width:     number of columns
 *      int height:    number of rows
 *      int size:      size of each cell in bytes
 *      int blocksize: number of cells along one side of a block
 *
 * Return: a new UArray2b_T; the caller must free it with UArray2b_free
 *
 * Expects
 *      width, height, size, and blocksize are all at least 1
 * Notes:
 *      Cell contents are initialized to zero
 *      Will raise error if width, height, or size is less than 1
 *      or blocksize is less than 1
 *      Will raise error if malloc fails
 *      Will raise error if UArray or UArray2 allocation fails
 * 
 ************************/

T UArray2b_new (int width, int height, int size, int blocksize)
{   
        if (width <= 0 or height <= 0 or size <= 0 or blocksize < 1) {
                RAISE(Args_Error_b);
        }
    
        // allocating space for uarray2
        T uarray2b = malloc(sizeof(*uarray2b));

        if (uarray2b == NULL) {
                RAISE(Memory_Error_b);
        }

        // Asssigning values to uarray2 struct
        uarray2b->width = width; 
        uarray2b->height = height; 
        uarray2b->size = size; 
        uarray2b->blocksize = blocksize; 


        int block_w = (width  + blocksize - 1) / blocksize;
        int block_h = (height + blocksize - 1) / blocksize;
        uarray2b->elems = UArray2_new(block_w, block_h, sizeof(UArray_T));

        for (int row_num = 0; row_num < block_h; row_num++) {
                for (int col_num = 0; col_num < block_w; col_num++) {
                        UArray_T *slot = UArray2_at(uarray2b->elems, 
                                                        col_num, row_num);
                        *slot = UArray_new(blocksize * blocksize, size);
                }
        }

        return uarray2b;
}


/********** UArray2b_new_64K_block ********
 *
 * Creates a new blocked 2D array whose blocksize is as large as possible
 * while keeping each block at most 64KB
 *
 * Parameters:
 *        int width:  number of columns
 *        int height: number of rows
 *        int size:   size of each cell in bytes
 *
 * Return: a new UArray2b_T; the caller must free it with UArray2b_free
 *
 * Expects
 *        width, height, and size are all at least 1
 * Notes:
 *        If one cell is larger than 64KB, blocksize is 1
 *        Will raise error (Args_Error_b) if width, height, or size is less 
 *        than 1
 *        Will raise error if memory allocation fails
 * 
 ************************/

T UArray2b_new_64K_block(int width, int height, int size)
{
        if (width <= 0 or height <= 0 or size <= 0) {
                RAISE(Args_Error_b);
        }

        int blocksize = 256 / sqrt(size); 
        if (blocksize < 1) {
                blocksize = 1;
        }

        return UArray2b_new(width, height, size, blocksize);
}


/********** UArray2b_free ********
 *
 * Frees all memory used by a blocked 2D array
 *
 * Parameters:
 *      T *array2b: address of the array to free
 *
 * Return: none
 *
 * Expects
 *      array2b and *array2b are not NULL
 * Notes:
 *      Frees every block, the UArray2 of blocks, and the struct
 *      Sets *array2b to NULL
 *      Will raise error if array2b or *array2b is NULL
 * 
 ************************/

void UArray2b_free (T *array2b)
{
        assert(array2b != NULL and *array2b != NULL);

        UArray2_T blocks = (*array2b)->elems;
        
        // Free each Hanson UArray block in the 2D array
        for (int row = 0; row < UArray2_height(blocks); row++) {
                for (int col = 0; col < UArray2_width(blocks); col++) {
                        UArray_free(UArray2_at(blocks, col, row));
                }
        }

        // Free the 2D array in the struct
        UArray2_free(&blocks);
        // Free the struct
        free(*array2b);
        *array2b = NULL;
}

/********** UArray2b_width ********
 *
 * Returns the number of columns in a blocked 2D array
 *
 * Parameters:
 *      T array2b: the array
 *
 * Return: the width of the array in cells
 *
 * Expects
 *      array2b is not NULL
 * Notes:
 *      Will raise error (Memory_Error_b) if array2b is NULL
 * 
 ************************/

int UArray2b_width (T array2b)
{
        if (array2b == NULL) {
            RAISE(Memory_Error_b);
        }

        int width = array2b->width;
        assert(width > 0);
        return width;
}


/********** UArray2b_height ********
 *
 * Returns the number of rows in a blocked 2D array
 *
 * Parameters:
 *        T array2b: the array
 *
 * Return: the height of the array in cells
 *
 * Expects
 *        array2b is not NULL
 * Notes:
 *        Will raise error (Memory_Error_b) if array2b is NULL
 * 
 ************************/

int UArray2b_height (T array2b)
{
        if (array2b == NULL) {
                RAISE(Memory_Error_b);
        }

        int height = array2b->height;
        assert(height > 0);
        return height;
}


/********** UArray2b_size ********
 *
 * Returns the size in bytes of one cell of a blocked 2D array
 *
 * Parameters:
 *        T array2b: the array
 *
 * Return: the size of one cell in bytes
 *
 * Expects
 *        array2b is not NULL
 * Notes:
 *        Will raise error (Memory_Error_b) if array2b is NULL
 * 
 ************************/

int UArray2b_size (T array2b)
{
        if (array2b == NULL) {
                RAISE(Memory_Error_b);
        }

        int size = array2b->size;
        assert(size > 0);
        return size;
}


/********** UArray2b_blocksize ********
 *
 * Returns the blocksize of a blocked 2D array
 *
 * Parameters:
 *        T array2b: the array
 *
 * Return: the number of cells along one side of a block
 *
 * Expects
 *        array2b is not NULL
 * Notes:
 *        Will raise error (Memory_Error_b) if array2b is NULL
 * 
 ************************/

int UArray2b_blocksize(T array2b)
{
        if (array2b == NULL) {
                RAISE(Memory_Error_b);
        }

        int blocksize = array2b->blocksize;
        assert(blocksize > 0);
        return blocksize;
}

/********** UArray2b_at ********
 *
 * Returns a pointer to the cell at the given column and row
 *
 * Parameters:
 *        T array2b:  the array
 *        int column: column of the cell
 *        int row:    row of the cell
 *
 * Return: a pointer to the cell, which the client may read or write
 *         through; it stays valid until the array is freed
 *
 * Expects
 *        array2b is not NULL
 *        0 <= column < width and 0 <= row < height
 * Notes:
 *        Will raise error (Memory_Error_b) if array2b is NULL
 *        Will raise error (Bound_Error_b) if column or row is out of range
 * 
 ************************/

void *UArray2b_at(T array2b, int column, int row)
{
        if (array2b == NULL) {
               RAISE(Memory_Error_b);
        }

        int width = UArray2b_width(array2b);
        int height = UArray2b_height(array2b);
        int blocksize = UArray2b_blocksize(array2b);
              
        // Out of bound error if col or row is not an expected value
        if (column < 0 or column >= width or row < 0 or row >= height) {
                RAISE(Bound_Error_b);
        } 
        
        int block_row = row / blocksize;
        int block_col = column / blocksize;
        UArray_T block = *(UArray_T *)UArray2_at(array2b->elems, block_col, 
                                                                block_row);
        return UArray_at(block,  
                        blocksize * (row % blocksize) + (column % blocksize));
}


/********** UArray2b_map ********
 *
 * Calls apply on every cell of the array, visiting every cell in one
 * block before moving to the next block
 *
 * Parameters:
 *        T array2b:  the array
 *        void apply: function called on each cell with its column, row,
 *                    the array, a pointer to the cell, and cl
 *        void *cl:   closure passed through to every call of apply
 *
 * Return: none
 *
 * Expects
 *        array2b and apply are not NULL
 * Notes:
 *        Blocks are visited in row-major order, and cells within a block
 *        are visited in row-major order
 *        Cells in edge blocks that lie outside width and height are skipped
 *        Will raise error (Memory_Error_b) if array2b is NULL
 * 
 ************************/
 
void UArray2b_map(T array2b, 
                void apply(int col, int row, T array2b, void *elem, void *cl),
                void *cl)
{
        if (array2b == NULL) {
                RAISE(Memory_Error_b);
        }

        int blocksize = array2b->blocksize;
        int b_height = UArray2_height(array2b->elems);
        int b_width = UArray2_width(array2b->elems);

        for (int row_num = 0; row_num < b_height; row_num++) {
                for (int col_num = 0; col_num < b_width; col_num++) {
                        // set p1 to the current block in the UArray2 elems
                        UArray_T p1 = *(UArray_T *)UArray2_at(array2b->elems, 
                                                        col_num, row_num);
                        for (int box_num = 0; 
                                box_num < blocksize * blocksize; box_num++) {
                        // calculating col and row for the elem and apply
                                int col = col_num * blocksize 
                                                        + box_num % blocksize;
                                int row = row_num * blocksize 
                                                        + box_num / blocksize;
                                // bound check against width and height
                                if (col < array2b->width and 
                                                row < array2b->height) {        
                                        apply(col, row, array2b, 
                                                UArray_at(p1, box_num), cl);
                                }
                        }               
                }
        }
}