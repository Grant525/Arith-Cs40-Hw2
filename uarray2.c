/***************************************************************  
 *    
 *        File Name: uarray2.c
 * 
 *        Assignmnet: iii
 *        Authors: Thee Thakong (tthako01) and Grant Caile (gcaile01)
 *        Due Date: October 1st, 2026
 * 
 *        Summary:
 *        Implementation of UArray2_T, a two-dimensional unboxed array
 *        that can hold elements of any type. The client sets the width,
 *        the height and the size in bytes of each element.
 *
 *        Elements are stored in row-major order in a single Hanson
 *        UArray_T. The element at (col, row) is at index
 *        row * width + col.
 *
 *        The interface provides functions to:
 *        - create and free a 2D array
 *        - get its width, height and element size
 *        - get a pointer to the element at a given column and row
 *        - apply a function to every element, in either row-major
 *        or column-major order
 *
 *        Raises a checked runtime error if a dimension or the element
 *        size is negative, if memory allocation fails, if a NULL array
 *        is passed in, or if an index is out of bounds.
 * 
***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <iso646.h>
#include <assert.h>

#include "uarray.h"
#include "except.h"
#include "uarray2.h"

Except_T Memory_Error = { "Memory allocation failed" };
Except_T Bound_Error = { "Out of bounds reference" };
Except_T Args_Error = { "Unexpected arguments" };

/* UArray2_T
 * Stores the 2D Uarray and its information
 * 
 * Members:
 *      int width: width of the 2D Uarray
 *      int height: height of the 2D Uarray
 *      int size: number of bytes for one element
 *      UArray_T elems: Hanson's 1D UArray
 */
struct UArray2_T {
        int width; 
        int height;
        int size;
        UArray_T elems;
};

/* uarray2_new
* Summary: Allocates space for and creates a new 2D array, using the given
*        width, height, and size of each element
*
* Parameters: int width: width of the 2D array
*        int height: height of the 2D array
*        int size: byte size of each element in the array
*
* Return: A pointer to the 2D array
* 
* Expects: width, height, and size are positive integers
*        caller is expected to free the allocated memory on the heap
*        after calling the function.
*/
UArray2_T UArray2_new(int width, int height, int size)
{
        if (width <= 0 or height <= 0 or size <= 0) {
                RAISE(Args_Error);
        }
        
        // allocating space for uarray2
        size_t length = width * height;
        UArray2_T uarray2 = malloc(sizeof(*uarray2));

        if (uarray2 == NULL) {
                RAISE(Memory_Error);
        }

        // Asssigning values to uarray2 struct
        uarray2->width = width;
        uarray2->height = height;
        uarray2->size = size;
        // Creating a Hanson UArray to represent the 2D array
        uarray2->elems =  UArray_new(length, size);

        return uarray2;
}

/* uarray2_width
* Summary: Goes through the 2Darray and returns the number of columns 
*        (or length of each row)
* 
* Parameters: T array: pointer to the 2D array
*
* Return: Returns an integer representing the length of each row in the array 
*
* Expects: The 2D array is not empty (array is not null). An exception is
*        raised if that is the case. 
*/
int UArray2_width(UArray2_T uarray2)
{
        if (uarray2 == NULL){
            RAISE(Memory_Error);
        }

        int width = uarray2->width;
        assert(width > 0);
        return width;
}


/* UArray2_height
* Summary: Goes through the 2Darray and returns the number of rows 
*        (or length of each column)
* 
* Parameters: T uarray2: pointer to the 2D array
*
* Return: Returns an integer representing the number of rows in the array
*
* Expects: The 2D array is not empty (array is not null). An exception is
*        raised if that is the case. 
*/
int UArray2_height(UArray2_T uarray2)
{
        if (uarray2 == NULL) {
                RAISE(Memory_Error);
        }

        int height = uarray2->height;
        assert(height > 0);
        return height;
}

/* uarray2_size
* Summary: Check for the size each element is taking up in the array
* 
* Parameters: T uarray2: pointer to the 2D array
*
* Return: Returns an integer representing the size of each element in the array
*
* Expects: The 2D array is not empty (array is not null). An exception is
*        raised if that is the case. 
*/
int UArray2_size(UArray2_T uarray2)
{
        if (uarray2 == NULL) {
                RAISE(Memory_Error);
        }

        int size = uarray2->size;
        assert(size > 0);
        return size;
}

/* uarray2_at
* Summary: Return the element associated with the given col and row coordinate
* 
* Parameters: T uarray2: pointer to the 2D array
*          int row: height position of the desired element
*          int col: width position of the desired element
*
* Return: pointer value of the element in the 2D array at the given width
*        and height (x,y)
* 
* Expects: The 2D array is not empty (array is not null). An exception is
*        raised if that is the case. 
*/
void *UArray2_at(UArray2_T uarray2, int col, int row)
{           
        if (uarray2 == NULL) {
               RAISE(Memory_Error);
        }

        int width = UArray2_width(uarray2);
        int height = UArray2_height(uarray2);

        // Out of bound error if col or row is not an expected value
        if (col < 0 or col >= width or row < 0 or row >= height) {
                RAISE(Bound_Error);
        }   
        
        // calculating the position of element based on col and row values
        int i = (row * width) + (col);
        return UArray_at(uarray2->elems, i);
}

/* uarray2_map_col_major
* Summary: Maps an apply function to each element in the array column by column
* 
* Parameters: T array: pointer to the 2D array
*        void apply(): function being applied to each element in the 2D array
*                - col: width position of the starting element
*                - row: height position of the starting element
*                - uarray2: pointer to the 2D array
*                - p1: pointer to the element being appplied
*                - p2: closure pointer
*        *cl: closure pointer
*
* Return: Nothing
*
* Expects:  The 2D array is not NULL. All columns are the same length
* as each other. All rows * are the same length as each other. Apply() takes
* in an array and returns some value. 
*/
void UArray2_map_col_major(UArray2_T uarray2, 
        void apply(int col, int row, UArray2_T uarray2, void *p1, void *p2),
        void *cl)
{
        if (uarray2 == NULL) {
                RAISE(Memory_Error);
        }

        for (int col_num = 0; col_num < uarray2->width; col_num++) {
                for (int row_num = 0; row_num < uarray2->height; row_num++){
                        // set p1 to the current element
                        void *p1 = UArray2_at(uarray2, col_num, row_num);
                        apply(col_num, row_num, uarray2, p1, cl);  
                }
        }
}

/* uarray2_map_row_major
* Summary: Maps an apply function to each element in the array row by row 
* 
* Parameters: T array: pointer to the 2D array
*        void apply(): function being applied to each element in the 2D array
*                - col: width position of the starting element
*                - row: height position of the starting element
*                - uarray2: pointer to the 2D array
*                - p1: pointer to the element being appplied
*                - p2: closure pointer
*        *cl: closure pointer
*
* Return: Nothing
*
* Expects:  The 2D array is not NULL. All columns are the same length
* as each other. All rows * are the same length as each other. Apply() takes
* in an array and returns some value. 
*/
void UArray2_map_row_major(UArray2_T uarray2, 
        void apply(int col, int row, UArray2_T uarray2, void *p1, void *p2),
        void *cl)
{
        if (uarray2 == NULL) {
                RAISE(Memory_Error);
        }

        for (int row_num = 0; row_num < uarray2->height; row_num++) {
                for (int col_num = 0; col_num < uarray2->width; col_num++){
                        // set p1 to the current element
                        void *p1 = UArray2_at(uarray2, col_num, row_num);
                        apply(col_num, row_num, uarray2, p1, cl);  
                }
        }
}

/* uarray2_free
* Summary: Deallocates all of the allocated memory for the 2D array.
* 
* Parameters: T array: Pointer to the pointer 2D array
*
* Return: nothing
*
* Expects: The 2D array is not empty (array is not null) and has allocated
*        memory. 
*/
void UArray2_free(UArray2_T *uarray2)
{
        assert(uarray2 != NULL and *uarray2 != NULL);

        // Free the Hanson UArray in the struct
        UArray_free(&(*uarray2)->elems);
        // Free the 2D Uarray
        free(*uarray2);
        *uarray2 = NULL;
}