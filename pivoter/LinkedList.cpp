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

#include"LinkedList.h"
#include"misc.h"
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>



/** \name Functions on Link structures
    \brief These functions operate on Link structures.
 */

//@{

/*! \brief tells if a given Link is the head sentinel.

    \param list A link structure.

    \return true if the link is the head sentinel, false otherwise.
*/

int isHead(Link* list)
{
    assert(list != nullptr);

    return (list->prev == nullptr);
}

/*! \brief tells if a given Link is the tail sentinel.

    \param list A link.

    \return true if the link is the tail sentinel, false otherwise.
*/

int isTail(Link* list)
{
    assert(list != nullptr);

    return (list->next == nullptr);
}

/*! \brief location-aware function to add a link after a given link.

    \param list The link that we want to add our data after.

    \param data A peice of data to put in the added link.

    \return A pointer to the Link that was added after list.
*/

Link* addAfter(Link* list, int data)
{
    assert(list != nullptr);
    assert(list->next != nullptr);

    Link* newLink = new Link;

    newLink->data = data;

    newLink->next = list->next;
    newLink->prev = list;

    list->next->prev = newLink;
    list->next = newLink;

    return newLink;
}

/*! \brief location-aware function to add a link before a given link.

    \param list The link that we want to add our data before. 

    \param data A peice of data to put in the added link.

    \return A pointer to the Link that was added after list.
*/

Link* addBefore(Link* list, int data)
{
    assert(list != nullptr);
    assert(list->prev != nullptr);

    Link* newLink = new Link;

    newLink->data = data;

    newLink->next = list;
    newLink->prev = list->prev;

    list->prev->next = newLink;
    list->prev = newLink;

    return newLink;
}

/*! \brief delete the given link, and return its data. 

    \param list The link that we want to get rid of.

    \return the data that was in the link, if it was 
            allocated by you, you need to free it..
*/

int deleteLink(Link* list)
{
    assert(list != nullptr);
    assert(list->next != nullptr);
    assert(list->prev != nullptr);

    int data = list->data;

    Link* linkToFree = removeLink(list);

    delete linkToFree;

    return data;
}

/*! \brief location-aware method to add a link before another link. 

    \param list The link that we want to add a link before.

    \param newLink The Link to be added after list.
*/

void addLinkBefore(Link* list, Link* newLink)
{
    assert(list != nullptr);
    assert(list->prev != nullptr);
    assert(newLink != nullptr);

    newLink->next = list;
    newLink->prev = list->prev;

    newLink->next->prev = newLink;
    newLink->prev->next = list->prev;
}

/*! \brief location-aware method to remove a link, and return it.

    \param list The link that we want remove and return.

    \return the removed link
*/

Link* removeLink(Link* list)
{
    assert(list != nullptr);
    assert(list->next != nullptr);
    assert(list->prev != nullptr);

    list->next->prev = list->prev;
    list->prev->next = list->next;

    list->next = nullptr;
    list->prev = nullptr;

    return list;
}

//@}

/** \name Functions on LinkedList structures
    \brief These functions operate on LinkedList structures.
 */

//@{

/*! \brief create a new empty linked list

    \return the created linked list
*/

LinkedList* createLinkedList()
{
    LinkedList* linkedList = new LinkedList;

    linkedList->head = new Link;
    linkedList->tail = new Link;

    linkedList->head->prev = nullptr;
    linkedList->head->next = linkedList->tail;
    linkedList->head->data = (int) 0xDEAD0000;

    linkedList->tail->prev = linkedList->head;
    linkedList->tail->next = nullptr;
    linkedList->tail->data = (int) 0xDEADFFFF;

    return linkedList;
}

/*! \brief destroy a linked list

    If you allocated data that is in each link, then
    this will cause a memory leak for you.

    \param linkedList The linked list to destroy.
*/

void destroyLinkedList(LinkedList* linkedList)
{
    Link* curr = linkedList->head;
    while (curr != nullptr) {
        Link* currNext = curr->next;
        delete curr;
        curr = currNext;
    }
    delete linkedList;
}

/*! \brief A location-aware function to add data to the beginning of a linked list.

    \param linkedList A linked list.

    \param data The data that we want to add to the beginning of linkedList.

    \return The link where data was placed in the linked list.
*/

Link* addFirst(LinkedList* linkedList, int data)
{
    assert(linkedList != nullptr);

    return addAfter(linkedList->head, data);
}

/*! \brief A location-aware function to add data to the end of a linked list.

    \param linkedList A linked list.

    \param data The data that we want to add to the end of linkedList.

    \return The link where data was placed in the linked list.
*/

Link* addLast(LinkedList* linkedList, int data)
{
    assert(linkedList != nullptr);

    return addBefore(linkedList->tail, data);
}

/*! \brief return the first piece of data in a linked list

    \param linkedList A linked list.

    \return The data in the first link of the linked list.
*/

int getFirst(LinkedList* linkedList)
{
    assert(linkedList != nullptr);
    assert(!isEmpty(linkedList));

    return linkedList->head->next->data;
}

/*! \brief Compute the number of data elements in a linked list.

    \param linkedList A linked list.

    \return The number of data elements in the linked list.
*/

int length(LinkedList* linkedList)
{
    int length = 0;
    Link* curr = linkedList->head->next;

    while(!isTail(curr))
    {
        length++;
        curr = curr->next;
    }

    return length;
}

/*! \brief Determine if a linked list is empty.

    \param linkedList A linked list.

    \return Non-zero if the linked list is empty, zero otherwise.
*/

int isEmpty(LinkedList* linkedList)
{
    assert(linkedList != nullptr);

    return isTail(linkedList->head->next);
}

//@}
