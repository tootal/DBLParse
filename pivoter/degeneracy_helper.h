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

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>

#include "misc.h"
#include "LinkedList.h"

/*! 
*/


/*! \struct NeighborList

    \brief For a given ordering, this stores later neighbors and earlier neighbors
           for a given vertex in linked lists.
*/

struct NeighborList
{
    int vertex; //!< the vertex that owns this neighbor list
    LinkedList* earlier; //!< a linked list of neighbors that come before this vertex in the ordering
    LinkedList* later; //!< a linked list of neighbors that come after this vertex in the ordering
    int orderNumber; //!< the position of this verex in the ordering
};

typedef struct NeighborList NeighborList;

/*! \struct NeighborListArray

    \brief For a given ordering, this stores later neighbors and earlier neighbors
           for a given vertex in arrays.

    This version of the NeighborList structure is more cache efficient.
*/

struct NeighborListArray
{
    int vertex; //!< the vertex that owns this neighbor list
    int* earlier; //!< an array of neighbors that come before this vertex in an ordering
    int earlierDegree; //!< the number of neighbors in earlier
    int* later; //!< an array of neighbors that come after this vertex in an ordering
    int laterDegree; //!< an array of neighbors that come after this vertex in an ordering
    int orderNumber; //!< the position of this verex in the ordering
};

typedef struct NeighborListArray NeighborListArray;

NeighborListArray** computeDegeneracyOrderArray(LinkedList** list, int size);
