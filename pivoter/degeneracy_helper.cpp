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

#include"misc.h"
#include"LinkedList.h"
#include"degeneracy_helper.h"

/*! \brief

    \param list an input graph, represented as an array of linked lists of integers

    \param size the number of vertices in the graph

    \return the degeneracy of the input graph.
*/

int computeDegeneracy(LinkedList** list, int size)
{
    int i = 0;

    int degeneracy = 0;
    
    // array of lists of vertices, indexed by degree
    LinkedList** verticesByDegree = (LinkedList**) calloc(size, sizeof(LinkedList*));

    // array of lists of vertices, indexed by degree
    Link** vertexLocator = (Link**) calloc(size, sizeof(Link*));

    int* degree = (int*) calloc(size, sizeof(int));

    for(i = 0; i < size; i++)
    {
        verticesByDegree[i] = createLinkedList();
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
            degeneracy = max(degeneracy,currentDegree);
            
            int vertex = (int)(size_t) getFirst(verticesByDegree[currentDegree]);

            deleteLink(vertexLocator[vertex]);

            degree[vertex] = -1;

            LinkedList* neighborList = list[vertex];

            Link* neighborLink = neighborList->head->next;

            while(!isTail(neighborLink))
            {
                int neighbor = (int)(size_t) neighborLink->data;

                if(degree[neighbor]!=-1)
                {
                    deleteLink(vertexLocator[neighbor]);

                    degree[neighbor]--;

                    if(degree[neighbor] != -1)
                    {
                        vertexLocator[neighbor] = 
                            addFirst(verticesByDegree[degree[neighbor]], 
                                     (int) neighbor);
                    }
                }

                neighborLink = neighborLink->next;
            }

            numVerticesRemoved++;
            currentDegree = 0;
        }
        else
        {
            currentDegree++;
        }

    }

    for(i = 0; i<size;i++)
    {
        destroyLinkedList(verticesByDegree[i]);
    }

    free(vertexLocator);
    free(verticesByDegree);
    free(degree);

    return degeneracy;
}

/*! \brief

    \param list an input graph, represented as an array of linked lists of integers

    \param size the number of vertices in the graph

    \return an array of NeighborLists representing a degeneracy ordering of the vertices.

    \see NeighborList
*/

NeighborList** computeDegeneracyOrderList(LinkedList** list, int size)
{
    NeighborList** ordering = (NeighborList**)calloc(size, sizeof(NeighborList*));

    int i = 0;

    int degeneracy = 0;
    
    // array of lists of vertices, indexed by degree
    LinkedList** verticesByDegree = (LinkedList**) calloc(size, sizeof(LinkedList*));

    // array of lists of vertices, indexed by degree
    Link** vertexLocator = (Link**) calloc(size, sizeof(Link*));

    int* degree = (int*) calloc(size, sizeof(int));

    for(i = 0; i < size; i++)
    {
        verticesByDegree[i] = createLinkedList();
        ordering[i] = (NeighborList*)malloc(sizeof(NeighborList));
        ordering[i]->earlier = createLinkedList();
        ordering[i]->later = createLinkedList();
    }

    // fill each cell of degree lookup table
    // then use that degree to populate the 
    // lists of vertices indexed by degree

    for(i=0; i<size; i++)
    {
        degree[i] = length(list[i]);
        //printf("degree[%d] = %d\n", i, degree[i]);
        vertexLocator[i] = addFirst(verticesByDegree[degree[i]], ((int) i));
    }
    
    int currentDegree = 0;

    int numVerticesRemoved = 0;

    while(numVerticesRemoved < size)
    {
        if(!isEmpty(verticesByDegree[currentDegree]))
        {
            degeneracy = max(degeneracy,currentDegree);
            
            int vertex = (int)(size_t) getFirst(verticesByDegree[currentDegree]);

            deleteLink(vertexLocator[vertex]);

            ordering[vertex]->vertex = vertex;
            ordering[vertex]->orderNumber = numVerticesRemoved;

            degree[vertex] = -1;

            LinkedList* neighborList = list[vertex];

            Link* neighborLink = neighborList->head->next;

            while(!isTail(neighborLink))
            {
                int neighbor = (int)(size_t) neighborLink->data;
                //printf("Neighbor: %d\n", neighbor);

                if(degree[neighbor]!=-1)
                {
                    deleteLink(vertexLocator[neighbor]);
                    addLast(ordering[vertex]->later, (int)neighbor);

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
        }
        else
        {
            currentDegree++;
        }

    }

    for(i = 0; i<size;i++)
    {
        destroyLinkedList(verticesByDegree[i]);
    }

    free(vertexLocator);
    free(verticesByDegree);
    free(degree);

    return ordering;
}

/*! \brief

    \param list an input graph, represented as an array of linked lists of integers

    \param size the number of vertices in the graph

    \return an array of NeighborListArrays representing a degeneracy ordering of the vertices.

    \see NeighborListArray
*/

NeighborListArray** computeDegeneracyOrderArray(LinkedList** list, int size)
{

    NeighborList** ordering = (NeighborList**)calloc(size, sizeof(NeighborList*));

    int i = 0;

    int degeneracy = 0;
    
    // array of lists of vertices, indexed by degree
    LinkedList** verticesByDegree = (LinkedList**) calloc(size, sizeof(LinkedList*));

    // array of lists of vertices, indexed by degree
    Link** vertexLocator = (Link**) calloc(size, sizeof(Link*));

    int* degree = (int*) calloc(size, sizeof(int));

    for(i = 0; i < size; i++)
    {
        verticesByDegree[i] = createLinkedList();
        ordering[i] = (NeighborList*)malloc(sizeof(NeighborList));
        ordering[i]->earlier = createLinkedList();
        ordering[i]->later = createLinkedList();
    }

    // fill each cell of degree lookup table
    // then use that degree to populate the 
    // lists of vertices indexed by degree
    // fprintf(stderr, "Ordering is:\n" );
    for(i=0; i<size; i++)
    {
        degree[i] = length(list[i]);
        // fprintf(stderr, "i=%d, degree[i]=%d\n",i, degree[i] );
        vertexLocator[i] = addFirst(verticesByDegree[degree[i]], ((int) i));
    }
    // fprintf(stderr, "Ordering is:\n" );
    int currentDegree = 0;

    int numVerticesRemoved = 0;

    while(numVerticesRemoved < size)
    {
        if(!isEmpty(verticesByDegree[currentDegree]))
        {
            degeneracy = max(degeneracy,currentDegree);
            
            int vertex = (int)(size_t) getFirst(verticesByDegree[currentDegree]);

            deleteLink(vertexLocator[vertex]);

            ordering[vertex]->vertex = vertex;
            ordering[vertex]->orderNumber = numVerticesRemoved;
            // fprintf(stderr, "vertex = %d, degree[vertex]=%d\n", vertex, degree[vertex]);
            degree[vertex] = -1;

            LinkedList* neighborList = list[vertex];

            Link* neighborLink = neighborList->head->next;

            while(!isTail(neighborLink))
            {
                int neighbor = (int)(size_t) (neighborLink->data);
                // fprintf(stderr, "neighbor = %d, degree[neighbor] = %d, ordering[neighbor]->orderNumber=%d\n",neighbor, degree[neighbor], ordering[neighbor]->orderNumber);
                if(degree[neighbor]!=-1)
                {
                    deleteLink(vertexLocator[neighbor]);
                    addLast(ordering[vertex]->later, (int)neighbor);

                    // fprintf(stderr, "In. neighbor = %d, degree[neighbor] = %d, ordering[neighbor]->orderNumber=%d\n",neighbor, degree[neighbor], ordering[neighbor]->orderNumber);
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
        }
        else
        {
            currentDegree++;
        }

    }
    
    NeighborListArray** orderingArray = (NeighborListArray**)calloc(size, sizeof(NeighborListArray*));

    for(i = 0; i<size;i++)
    {
        orderingArray[i] = (NeighborListArray*)malloc(sizeof(NeighborListArray));
        orderingArray[i]->vertex = ordering[i]->vertex;
        orderingArray[i]->orderNumber = ordering[i]->orderNumber;
        // if (orderingArray[i]->vertex == 175421) fprintf(stderr, "175421, orderNumber = %d\n", ordering[i]->orderNumber );
        // if (orderingArray[i]->vertex == 573) fprintf(stderr, "573, orderNumber = %d\n", ordering[i]->orderNumber );
        orderingArray[i]->laterDegree = length(ordering[i]->later);
        orderingArray[i]->later = (int *)calloc(orderingArray[i]->laterDegree, sizeof(int));

        int j=0;
        Link* curr = ordering[i]->later->head->next;
        while(!isTail(curr))
        {
            orderingArray[i]->later[j++] = (int)(size_t)(curr->data);
            curr = curr->next;
        }

        orderingArray[i]->earlierDegree = length(ordering[i]->earlier);
        orderingArray[i]->earlier = (int *)calloc(orderingArray[i]->earlierDegree, sizeof(int));

        j=0;
        curr = ordering[i]->earlier->head->next;
        while(!isTail(curr))
        {
            orderingArray[i]->earlier[j++] = (int)(size_t)(curr->data);
            curr = curr->next;
        }
    }

    // fprintf(stderr, "Ordering is:\n" );
    for(i = 0; i<size;i++)
    {
        // fprintf(stderr, "vertex = %d, orderNumber = %d, laterdeg = %d, earlierdeg = %d\n", orderingArray[i]->vertex, orderingArray[i]->orderNumber, orderingArray[i]->laterDegree, orderingArray[i]->earlierDegree );
        free(ordering[i]);
        destroyLinkedList(verticesByDegree[i]);
    }

    free(ordering);

    free(vertexLocator);
    free(verticesByDegree);
    free(degree);

    return orderingArray;
}
