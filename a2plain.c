/**************************************************************
 *
 *                     a2plain.c
 *
 *        Assignment: CS 40 HW 3, Locality
 *        Authors:    Theetat Thakong (tthako01),
 *                    Grant Caile (gcaile01)
 *        Date:       October 14, 2026
 * 
 *        Implementation of the A2Methods interface for
 *        plain unblocked 2D arrays. Each method is a wrapper
 *        around the matching UArray2 function, and the
 *        methods are collected into a struct exported as
 *        uarray2_methods_plain.
 *
 *        Plain arrays support row-major and column-major mapping but
 *        not block-major mapping, so the blocksize and block-major
 *        entries are NULL. The default map is row-major, which
 *        matches how UArray2 stores its elements.
 *
 **************************************************************/
 
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <iso646.h>
#include <assert.h>

#include <a2plain.h>
#include "uarray2.h"

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

/********** new ********
 *
 * Creates a new plain 2D array
 *
 * Parameters:
 *        int width:  number of columns
 *        int height: number of rows
 *        int size:   size of each cell in bytes
 *
 * Return: a new plain array; the caller must free it with a2free
 *
 * Expects
 *        width, height, and size are all at least 1
 * Notes:
 *        Will raise error if width, height, or size is invalid
 *        Will raise error if memory allocation fails
 * 
 ************************/
static A2Methods_UArray2 new(int width, int height, int size)
{
       
        return UArray2_new(width, height, size);
}


/********** new_with_blocksize ********
 *
 * Creates a new plain 2D array. Plain arrays have no blocks, so the
 * blocksize is ignored.
 *
 * Parameters:
 *        int width:     number of columns
 *        int height:    number of rows
 *        int size:      size of each cell in bytes
 *        int blocksize: ignored
 *
 * Return: a new plain array; the caller must free it with a2free
 *
 * Expects
 *        width, height, and size are all at least 1
 * Notes:
 *        Will raise error if width, height, or size is invalid
 *        Will raise error if memory allocation fails
 * 
 ************************/

static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        /* TODO:
         * Implement this function and remove the dummy return statement.
         */
        (void) blocksize;
        return UArray2_new(width, height, size);
}


/* TODO: ...many more private (static) definitions follow */

/********** a2free ********
 *
 * Frees all memory used by a plain 2D array
 *
 * Parameters:
 *        A2Methods_UArray2 *array2p: address of the array to free
 *
 * Return: none
 *
 * Expects
 *        array2p and *array2p are not NULL
 *        *array2p was created by this methods suite
 * Notes:
 *        Sets *array2p to NULL
 *       Will raise error if array2p or *array2p is NULL
 * 
 ************************/

static void a2free(A2Methods_UArray2 * array2p)
{
        UArray2_free((UArray2_T *) array2p);
}


/********** width ********
 *
 * Returns the number of columns in a plain 2D array
 *
 * Parameters:
 *        A2Methods_UArray2 array2: the array
 *
 * Return: the width of the array in cells
 *
 * Expects
 *        array2 is not NULL
 * Notes:
 *        Will raise error if array2 is NULL
 * 
 ************************/

static int width(A2Methods_UArray2 array2)
{
        return UArray2_width(array2);
}


/********** height ********
 *
 * Returns the number of rows in a plain 2D array
 *
 * Parameters:
 *        A2Methods_UArray2 array2: the array
 *
 * Return: the height of the array in cells
 *
 * Expects
 *        array2 is not NULL
 * Notes:
 *        Will raise error if array2 is NULL
 * 
 ************************/

static int height(A2Methods_UArray2 array2)
{
        return UArray2_height(array2);
}


/********** size ********
 *
 * Returns the size in bytes of one cell of a plain 2D array
 *
 * Parameters:
 *        A2Methods_UArray2 array2: the array
 *
 * Return: the size of one cell in bytes
 *
 * Expects
 *        array2 is not NULL
 * Notes:
 *        Will raise error if array2 is NULL
 * 
 ************************/

static int size(A2Methods_UArray2 array2)
{
        return UArray2_size(array2);
}


/********** at ********
 *
 * Returns a pointer to the cell at the given column and row
 *
 * Parameters:
 *        A2Methods_UArray2 array2: the array
 *        int col:                  column of the cell
 *        int row:                  row of the cell
 *
 * Return: a pointer to the cell, valid until the array is freed
 *
 * Expects
 *        array2 is not NULL
 *        0 <= col < width and 0 <= row < height
 * Notes:
 *        Will raise error if array2 is NULL or col or row is out of range
 * 
 ************************/

static A2Methods_Object *at(A2Methods_UArray2 array2, int col, int row)
{
        return UArray2_at(array2, col, row);
}


typedef void UArray2_applyfun(int i, int j, UArray2_T array2, 
                              void *elem, void *cl);

/********** map_row_major ********
 *
 * Calls apply on every cell in row-major order (each row from left to
 * right, rows from top to bottom)
 *
 * Parameters:
 *        A2Methods_UArray2 uarray2: the array
 *        A2Methods_applyfun apply:  function called on each cell with its
 *                                   column, row, the array, a pointer to
 *                                   the cell, and cl
 *        void *cl:                  closure passed through to apply
 *
 * Return: none
 *
 * Expects
 *        uarray2 and apply are not NULL
 * Notes:
 *        Also used as the default map for plain arrays
 *        Will raise error if uarray2 is NULL
 * 
 ************************/

static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_row_major(uarray2, (UArray2_applyfun *)apply, cl);
}


/********** map_col_major ********
 *
 * Calls apply on every cell in column-major order (each column from top
 * to bottom, columns from left to right)
 *
 * Parameters:
 *      A2Methods_UArray2 uarray2: the array
 *          A2Methods_applyfun apply:  function called on each cell with its
 *                                   column, row, the array, a pointer to
 *                                   the cell, and cl
 *        void *cl:                  closure passed through to apply
 *
 * Return: none
 *
 * Expects
 *        uarray2 and apply are not NULL
 * Notes:
 *        Will raise error if uarray2 is NULL
 * 
 ************************/

static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        UArray2_map_col_major(uarray2, (UArray2_applyfun *)apply, cl);
}


struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

/********** apply_small ********
 *
 * Adapter apply function that lets a small apply function (one that
 * takes only the element and closure) be used with the UArray2 map
 * functions
 *
 * Parameters:
 *        int i, int j:      column and row of the cell (unused)
 *        UArray2_T uarray2: the array (unused)
 *        void *elem:        pointer to the current cell
 *        void *vcl:         pointer to a struct small_closure
 *
 * Return: none
 *
 * Expects
 *        vcl is not NULL and holds a non-NULL apply function
 * Notes:
 *        Calls the small apply function on elem with the client's closure
 * 
 ************************/

static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        struct small_closure *cl = vcl;
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl);
}


/********** small_map_row_major ********
 *
 * Calls a small apply function on every cell in row-major order
 *
 * Parameters:
 *        A2Methods_UArray2 a2:          the array
 *        A2Methods_smallapplyfun apply: function called on each cell with
 *                                       a pointer to the cell and cl
 *        void *cl:                      closure passed through to apply
 *
 * Return: none
 *
 * Expects
 *        a2 and apply are not NULL
 * Notes:
 *        Also used as the default small map for plain arrays
 *        Will raise error if a2 is NULL
 * 
 ************************/

static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}


/********** small_map_col_major ********
 *
 * Calls a small apply function on every cell in column-major order
 *
 * Parameters:
 *        A2Methods_UArray2 a2:          the array
 *        A2Methods_smallapplyfun apply: function called on each cell with
 *                                       a pointer to the cell and cl
 *        void *cl:                      closure passed through to apply
 *
 * Return: none
 *
 * Expects
 *        a2 and apply are not NULL
 * Notes:
 *        Will raise error if a2 is NULL
 * 
 ************************/

static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}


static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free,
        width,
        height,
        size,
        NULL,
        at,
        map_row_major,                   // map_row_major
        map_col_major,                   // map_col_major
        NULL,
        map_row_major,                   // map_default
        small_map_row_major,             // small_map_row_major
        small_map_col_major,             // small_map_col_major
        NULL,
        small_map_row_major,             // small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
