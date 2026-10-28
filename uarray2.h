/***************************************************************
 *
 *        File Name: uarray2.h
 *
 *        Assignment: iii
 *        Authors: Thee Thakong (tthako01) and Grant Caile (gcaile01)
 *        Due Date: October 1st, 2026
 *
 *        Summary:
 *        Interface for UArray2_T, two-dimensional
 *        unboxed array that holds elements of any type. 
 *
 ***************************************************************/
#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T   
typedef struct T *T;

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
T UArray2_new(int width, int height, int size);

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
int UArray2_width(T uarray2);

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
int UArray2_height(T uarray2);


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
int UArray2_size(T uarray2);

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
void *UArray2_at(T uarray2, int col, int row);

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
void UArray2_map_col_major(T uarray2, 
        void apply(int col, int row, T uarray2, void *p1, void *p2),
        void *cl);

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
void UArray2_map_row_major(T uarray2, 
        void apply(int col, int row, T uarray2, void *p1, void *p2),
        void *cl);

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
void UArray2_free(T *uarray2);

#undef T
#endif
