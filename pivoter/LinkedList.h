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

struct Link;

/*! \struct LinkedList

    \brief Stores a linked list, with sentinel links for head and tail.
           These sentinels contain dummy values. 

*/

struct LinkedList
{
    struct Link* head; //!< head of the linked list, a dummy sentinel
    struct Link* tail; //!< tail of the linked list, a dummy sentinel
};

/*! \struct Link

    \brief Stores data and pointers to next and previous links.

*/

struct Link
{
    int data; //!< arbitrary data stored in the link
    struct Link* next; //!< the previous link in the chain
    struct Link* prev; //!< the next link in the chain
};

typedef struct Link Link;

typedef struct LinkedList LinkedList;

int isHead(Link* list);

int isTail(Link* list);

int deleteLink(Link* list);

Link* addAfter(Link* list, int data);

Link* addBefore(Link* list, int data);

void addLinkBefore(Link* list, Link* newLink);

Link* removeLink(Link* list);

LinkedList* createLinkedList(void);

void destroyLinkedList(LinkedList* linkedList);

int equal( LinkedList* list1, 
           LinkedList* list2, 
           int (*comparator)(int,int));

Link* addFirst(LinkedList* linkedList, int data);

Link* addLast(LinkedList* linkedList, int data);

int getFirst(LinkedList* linkedList);

int length(LinkedList* linkedList);

int isEmpty(LinkedList* linkedList);

