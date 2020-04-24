#pragma once

/* 
    This file contains the algorithm for listing all cliques
    according to the algorithm of Jain et al. specified in 
    "The power of pivoting for exact clique counting." (WSDM 2020).

    This code is a modified version of the code of quick-cliques-1.0 library for counting 
    maximal cliques by Darren Strash (first name DOT last name AT gmail DOT com).

    Original author: Darren Strash (first name DOT last name AT gmail DOT com)

    Copyright (c) 2011 Darren Strash. This code is released under the GNU Public License (GPL) 3.0.

    Modifications Copyright (c) 2020 Shweta Jain
    
    This program is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version. 
 
    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details. 
 
    You should have received a copy of the GNU General Public License 
    along with this program.  If not, see <http://www.gnu.org/licenses/> 
*/

#include<stdlib.h>


#ifdef MEMORY_DEBUG
#include<stdio.h>

/*! \brief Call malloc and ensure that it returns non-NULL.

    \param x, the number of bytes to allocate

    \return a pointer to the allocated memory. If it is null, exit the program with error.

*/

static void* MallocWithCheck(size_t x)
{
    #ifdef ALLOW_ALLOC_ZERO_BYTES
    void* retvalue = malloc(x);
    #else
    void* retvalue = malloc(max(x,1));
    #endif

    if(retvalue==NULL)
    {
        fprintf(stderr, "ERROR, malloc returned null pointer, that means we probably ran out of memory...\n");
        exit(1);
    }

    return retvalue;
};

/*! \brief Call calloc, and ensure that it returns non-NULL.

    \param x, the number of array slots to allocate

    \param x, the number of bytes to allocate in each slot

    \return a pointer to the allocated memory. If it is null, exit the program with error.

*/

static void* CallocWithCheck(size_t x, size_t y)
{
    #ifdef ALLOW_ALLOC_ZERO_BYTES
    void* retvalue = calloc(x,y); 
    #else
    void* retvalue = calloc(max(x,1),max(y,1)); 
    #endif

    if(retvalue==NULL)
    {
        fprintf(stderr, "ERROR, calloc returned null pointer, that means we probably ran out of memory...\n");
        exit(1);
    }

    return retvalue;
};

#define Malloc(x) MallocWithCheck(x)
#define Calloc(x,y) CallocWithCheck(x,y)
#define Free(x) free(x)

#else

    #ifdef ALLOW_ALLOC_ZERO_BYTES
    #define Malloc(x) malloc(x)
    #define Calloc(x,y) calloc(x,y)
    #define Free(x) free(x)

    #else

    #define Malloc(x) malloc(max(x,1))
    #define Calloc(x,y) calloc(max(x,1),max(y,1))
    #define Free(x) free(x)

    #endif // ALLOW_ALLOC_ZERO_BYTES
#endif // MEMORY_DEBUG


