/***************************************************************
 *
 *        File Name: uarray2b.h
 *
 *        Assignment: CS 40 HW 3, Locality
 *        Authors: Theetat Thakong (tthako01) and Grant Caile (gcaile01)
 *        Date: October 14, 2026
 *
 *        Summary:
 *        Interface for UArray2b_T, a blocked two-dimensional unboxed
 *        array that holds elements of any type. Cells are grouped into
 *        square blocks of blocksize x blocksize cells, and the cells of
 *        each block are stored contiguously in memory.
 *
 ***************************************************************/
#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#include "except.h"

#define T UArray2b_T
typedef struct T *T;

extern Except_T Memory_Error_b;
extern Except_T Bound_Error_b;
extern Except_T Args_Error_b;

/* UArray2b_new
* Summary: Allocates space for and creates a new blocked 2D array, using the
*        given width, height, size of each element, and blocksize
*
* Parameters: int width: number of columns
*        int height: number of rows
*        int size: byte size of each element in the array
*        int blocksize: number of cells along one side of a block
*
* Return: A pointer to the blocked 2D array; cells are initialized to zero
*
* Expects: width, height, size, and blocksize are positive integers. Raises
*        Args_Error_b otherwise, and Memory_Error_b if allocation fails.
*        Caller must free the array with UArray2b_free.
*/
T UArray2b_new(int width, int height, int size, int blocksize);

/* UArray2b_new_64K_block
* Summary: Creates a new blocked 2D array whose blocksize is as large as
*        possible while keeping each block at most 64KB
*
* Parameters: int width: number of columns
*        int height: number of rows
*        int size: byte size of each element in the array
*
* Return: A pointer to the blocked 2D array. If one cell is larger than 64KB,
*        the blocksize is 1.
*
* Expects: width, height, and size are positive integers. Raises
*        Args_Error_b otherwise. Caller must free the array with
*        UArray2b_free.
*/
T UArray2b_new_64K_block(int width, int height, int size);

/* UArray2b_free
* Summary: Deallocates all of the memory used by the blocked 2D array and
*        sets *array2b to NULL
*
* Parameters: T *array2b: pointer to the blocked 2D array
*
* Return: nothing
*
* Expects: array2b and *array2b are not NULL (checked runtime error).
*/
void UArray2b_free(T *array2b);

/* UArray2b_width
* Summary: Returns the number of columns in the blocked 2D array
*
* Parameters: T array2b: pointer to the blocked 2D array
*
* Return: The width of the array in cells
*
* Expects: array2b is not NULL. Raises Memory_Error_b otherwise.
*/
int UArray2b_width(T array2b);

/* UArray2b_height
* Summary: Returns the number of rows in the blocked 2D array
*
* Parameters: T array2b: pointer to the blocked 2D array
*
* Return: The height of the array in cells
*
* Expects: array2b is not NULL. Raises Memory_Error_b otherwise.
*/
int UArray2b_height(T array2b);

/* UArray2b_size
* Summary: Returns the size in bytes of one element of the blocked 2D array
*
* Parameters: T array2b: pointer to the blocked 2D array
*
* Return: The size of one element in bytes
*
* Expects: array2b is not NULL. Raises Memory_Error_b otherwise.
*/
int UArray2b_size(T array2b);

/* UArray2b_blocksize
* Summary: Returns the blocksize of the blocked 2D array
*
* Parameters: T array2b: pointer to the blocked 2D array
*
* Return: The number of cells along one side of a block
*
* Expects: array2b is not NULL. Raises Memory_Error_b otherwise.
*/
int UArray2b_blocksize(T array2b);

/* UArray2b_at
* Summary: Returns a pointer to the element at the given column and row
*
* Parameters: T array2b: pointer to the blocked 2D array
*        int column: column of the desired element
*        int row: row of the desired element
*
* Return: Pointer to the element, valid until the array is freed
*
* Expects: array2b is not NULL (raises Memory_Error_b), and
*        0 <= column < width and 0 <= row < height (raises Bound_Error_b).
*/
void *UArray2b_at(T array2b, int column, int row);

/* UArray2b_map
* Summary: Maps an apply function to each element in the array, visiting
*        every cell in one block before moving to the next block
*
* Parameters: T array2b: pointer to the blocked 2D array
*        void apply(): function applied to each element in the array
*                - col: column of the element
*                - row: row of the element
*                - array2b: pointer to the blocked 2D array
*                - elem: pointer to the element being applied
*                - cl: closure pointer
*        void *cl: closure pointer passed to every call of apply
*
* Return: Nothing
*
* Expects: array2b and apply are not NULL. Raises Memory_Error_b if array2b
*        is NULL. Cells in edge blocks that lie outside the width and height
*        are not visited.
*/
void UArray2b_map(T array2b,
        void apply(int col, int row, T array2b, void *elem, void *cl),
        void *cl);

#undef T
#endif
