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

#include<limits.h>
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>

#include <QVector>

#include"misc.h"
#include"LinkedList.h"
#include"degeneracy_helper.h"

/*! \brief

    \param list an input graph, represented as an array of linked lists of integers

    \param size the number of vertices in the graph

    \return an array of NeighborListArrays representing a degeneracy ordering of the vertices.

    \see NeighborListArray
*/

void computeDegeneracyOrderArray(QVector<LinkedList*> &list,
                                 int size,
                                 QVector<NeighborListArray*> &orderingArray)
{
    qDebug(__FUNCTION__);
    QVector<NeighborList*> ordering(size);
    
    int i = 0;

    int degeneracy = 0;
    
    // array of lists of vertices, indexed by degree
    QVector<LinkedList*> verticesByDegree(size);
    
    // array of lists of vertices, indexed by degree
    QVector<Link*> vertexLocator(size);
    
    QVector<int> degree(size);
    
    for(i = 0; i < size; i++) {
        verticesByDegree[i] = createLinkedList();
        ordering[i] = new NeighborList;
        ordering[i]->earlier = createLinkedList();
        ordering[i]->later = createLinkedList();
    }

    // fill each cell of degree lookup table
    // then use that degree to populate the 
    // lists of vertices indexed by degree
    for(i=0; i<size; i++)
    {
        degree[i] = length(list[i]);
        vertexLocator[i] = addFirst(verticesByDegree[degree[i]], ((int) i));
    }
    int currentDegree = 0;

    int numVerticesRemoved = 0;

    while(numVerticesRemoved < size)
    {
        if(!isEmpty(verticesByDegree[currentDegree]))
        {
            degeneracy = std::max(degeneracy,currentDegree);
            
            int vertex = getFirst(verticesByDegree[currentDegree]);

            deleteLink(vertexLocator[vertex]);

            ordering[vertex]->vertex = vertex;
            ordering[vertex]->orderNumber = numVerticesRemoved;
            degree[vertex] = -1;

            LinkedList* neighborList = list[vertex];

            Link* neighborLink = neighborList->head->next;

            while(!isTail(neighborLink)) {
                int neighbor = neighborLink->data;
                if(degree[neighbor]!=-1)
                {
                    deleteLink(vertexLocator[neighbor]);
                    addLast(ordering[vertex]->later, neighbor);
                    degree[neighbor]--;

                    if(degree[neighbor] != -1)
                    {
                        vertexLocator[neighbor] = 
                            addFirst(verticesByDegree[degree[neighbor]], 
                                     (int) neighbor);
                    }
                }
                else
                {
                    addLast(ordering[vertex]->earlier, (int) neighbor);
                }

                neighborLink = neighborLink->next;
            }

            numVerticesRemoved++;
            currentDegree = 0;
        } else {
            currentDegree++;
        }

    }
    orderingArray.resize(size);

    for(i = 0; i < size; i++) {
        orderingArray[i] = new NeighborListArray;
        orderingArray[i]->vertex = ordering[i]->vertex;
        orderingArray[i]->orderNumber = ordering[i]->orderNumber;
        orderingArray[i]->laterDegree = length(ordering[i]->later);
        orderingArray[i]->later = new int[orderingArray[i]->laterDegree];

        int j=0;
        Link *curr = ordering[i]->later->head->next;
        while(!isTail(curr)) {
            orderingArray[i]->later[j++] = curr->data;
            curr = curr->next;
        }

        orderingArray[i]->earlierDegree = length(ordering[i]->earlier);
        orderingArray[i]->earlier = new int[orderingArray[i]->earlierDegree];

        j=0;
        curr = ordering[i]->earlier->head->next;
        while(!isTail(curr)) {
            orderingArray[i]->earlier[j++] = curr->data;
            curr = curr->next;
        }
    }
    for(i = 0; i<size;i++) {
        destroyLinkedList(ordering[i]->earlier);
        destroyLinkedList(ordering[i]->later);
        delete ordering[i];
        destroyLinkedList(verticesByDegree[i]);
    }
}
