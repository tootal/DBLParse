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
#include<time.h>
#include<stdlib.h>
#include<string.h>

#include <vector>

#include <bignumber.h>

#include"misc.h"
#include"LinkedList.h"
#include"degeneracy_helper.h"

std::vector<std::vector<BigNumber>> nCr;

void populate_nCr()
{
    nCr.resize(1001);
    for(int row = 0; row < 1001; ++row)
    {
        nCr[row].resize(401);
        for (int col = 0; col < 401; ++col)
        {
            if (row == 0 || col == 0 || row == col) nCr[row][col] = 1;
            else nCr[row][col] = nCr[row-1][col] + nCr[row-1][col-1];
        }
    }
}

/*! \brief destroy a linked list of integer arrays that have
           -1 in the last cell, have have been allocated by
           the user.

    \param cliques the linked list of integer arrays
*/

void destroyCliqueResults(LinkedList* cliques)
{
    Link* curr = cliques->head->next;
    while(!isTail(curr))
    {
        int* clique = (int*)curr->data;
        free(clique);
        curr = curr->next;
    } 

    destroyLinkedList(cliques); 
}

void runAndPrintStatsCliques(LinkedList** adjListLinked, int n)
{
    fflush(stderr);
    int max_k = 0;
    
    int deg = 0, m = 0;
    FILE *fp;

    fflush(stdout);
    fopen_s (&fp, "authors_cliques.txt", "w");
    if (!fp) printf("Could not open output file.\n");
    fflush(stdout);

    NeighborListArray** orderingArray = computeDegeneracyOrderArray(adjListLinked, n);
    fflush(stdout);
    for (int i=0; i<n; i++)
    {
        if (deg < orderingArray[i]->laterDegree) deg = orderingArray[i]->laterDegree;
        m += orderingArray[i]->laterDegree;
    }

    max_k = deg + 1;
    double nCalls{};
    double sumP{};
    double sqP{};
    std::vector<BigNumber> cliqueCounts(max_k + 1);
    listAllCliquesDegeneracy_A(cliqueCounts, orderingArray, n, max_k, &nCalls, &sumP, &sqP);
   
    while (cliqueCounts[max_k] == 0) max_k--;
    fprintf(fp, "%d\n", max_k);
    BigNumber totalCliques = 0;
    for (int i=1; i<=max_k; i++)
    {
        fprintf(fp, "%d %s\n", i, cliqueCounts[i].getString().c_str()); 
        totalCliques += cliqueCounts[i];
    }

    fprintf(fp, "%s\n", totalCliques.getString().c_str());
    fclose(fp);
    free(orderingArray);
    nCr.clear();
}

/*! \brief Computes the vertex v in P union X that has the most neighbors in P,
           and places P \ {neighborhood of v} in an array. These are the 
           vertices to consider adding to the partial clique during the current
           recursive call of the algorithm.

    \param pivotNonNeighbors  An intially unallocated pointer, which will contain the set 
                              P \ {neighborhood of v} when this function completes.

    \param numNonNeighbors A pointer to a single integer, which has been preallocated,
                           which will contain the number of elements in pivotNonNeighbors.
 
    \param vertexSets An array containing sets of vertices divided into sets X, P, R, and other.
 
    \param vertexLookup A lookup table indexed by vertex number, storing the index of that 
                        vertex in vertexSets.

    \param neighborsInP Maps vertices to arrays of neighbors such that 
                        neighbors in P fill the first cells

    \param numNeighbors An the neighbor of neighbors a vertex had in P,
                        the first time this function is called, this bound is 
                        used to keep us from allocating more than linear space.
 
    \param beginX The index where set X begins in vertexSets.
 
    \param beginP The index where set P begins in vertexSets.

    \param beginR The index where set R begins in vertexSets.

*/

int findBestPivotNonNeighborsDegeneracyCliques( int** pivotNonNeighbors, int* numNonNeighbors,
                                                int* vertexSets, int* vertexLookup,
                                                int** neighborsInP, int* numNeighbors,
                                                int , int beginP, int beginR)
{
    int pivot = -1;
    int maxIntersectionSize = -1;

    // iterate over each vertex in P union X 
    // to find the vertex with the most neighbors in P.
    int j = beginP;
    while(j<beginR)
    {
        int vertex = vertexSets[j];
        int numPotentialNeighbors = std::min(beginR - beginP, numNeighbors[vertex]); //bug resolved by Shweta

        int numNeighborsInP = 0;

        int k = 0;
        while(k<numPotentialNeighbors)
        {
            int neighbor = neighborsInP[vertex][k];
            int neighborLocation = vertexLookup[neighbor];

            if(neighborLocation >= beginP && neighborLocation < beginR)
            {
                numNeighborsInP++;
            }
            else
            {
                break;
            }

            k++;
        }

        if(numNeighborsInP > maxIntersectionSize)
        {
            pivot = vertex;
            maxIntersectionSize = numNeighborsInP;
        }

        j++;
    }

    // compute non neighbors of pivot by marking its neighbors
    // and moving non-marked vertices into pivotNonNeighbors.
    // we must do this because this is an efficient way
    // to compute non-neighbors of a vertex in 
    // an adjacency list.

    // we initialize enough space for all of P; this is
    // slightly space inefficient, but it results in faster
    // computation of non-neighbors.
    *pivotNonNeighbors = (int *)calloc(beginR-beginP, sizeof(int));
    memcpy(*pivotNonNeighbors, &vertexSets[beginP], (beginR-beginP)*sizeof(int));

    // we will decrement numNonNeighbors as we find neighbors
    *numNonNeighbors = beginR-beginP;

    int numPivotNeighbors = std::min(beginR - beginP, numNeighbors[pivot]); //bug resolved by Shweta
  
    // mark the neighbors of pivot that are in P.
    j = 0;
    while(j<numPivotNeighbors)
    {
        int neighbor = neighborsInP[pivot][j];
        int neighborLocation = vertexLookup[neighbor];

        if(neighborLocation >= beginP && neighborLocation < beginR)
        {
            (*pivotNonNeighbors)[neighborLocation-beginP] = -1;
        }
        else
        {
            break;
        }

        j++;
    }

    // move non-neighbors of pivot in P to the beginning of
    // pivotNonNeighbors and set numNonNeighbors appriopriately.

    // if a vertex is marked as a neighbor, the we move it
    // to the end of pivotNonNeighbors and decrement numNonNeighbors.
    j = 0;
    while(j<*numNonNeighbors)
    {
        int vertex = (*pivotNonNeighbors)[j];

        if(vertex == -1)
        {
            (*numNonNeighbors)--;
            (*pivotNonNeighbors)[j] = (*pivotNonNeighbors)[*numNonNeighbors];
            continue;
        }

        j++;
    }

    return pivot; 
}

/*! \brief Move vertex to R, set P to vertex's later neighbors and
           set X to vertex's earlier neighbors.

    \param vertex The vertex to move to R.

    \param orderNumber The position of vertex in the ordering.

    \param vertexSets An array containing sets of vertices divided into sets X, P, and other.
 
    \param vertexLookup A lookup table indexed by vertex number, storing the index of that 
                        vertex in vertexSets.

    \param orderingArray A degeneracy order of the input graph.

    \param neighborsInP Maps vertices to arrays of neighbors such that 
                        neighbors in P fill the first cells

    \param numNeighbors An the neighbor of neighbors a vertex had in P,
                        the first time this function is called, this bound is 
                        used to keep us from allocating more than linear space.
 
    \param pBeginX The index where set X begins in vertexSets.
 
    \param pBeginP The index where set P begins in vertexSets.

    \param pBeginR The index where set R begins in vertexSets.

    \param pNewBeginX After function, contains the new index where set X begins
                      in vertexSets after adding vertex to R.
 
    \param pNewBeginP After function, contains the new index where set P begins
                      in vertexSets after adding vertex to P.

    \param pNewBeginR After function, contains the new index where set R begins
                      in vertexSets after adding vertex to R.
*/

void fillInPandXForRecursiveCallDegeneracyCliques( int vertex, int orderNumber,
                                                   int* vertexSets, int* vertexLookup, 
                                                   NeighborListArray** orderingArray,
                                                   int** neighborsInP, int* numNeighbors,
                                                   int*, int *, int *pBeginR, 
                                                   int* pNewBeginX, int* pNewBeginP, int *pNewBeginR)
{
    int vertexLocation = vertexLookup[vertex];

    (*pBeginR)--;
    vertexSets[vertexLocation] = vertexSets[*pBeginR];
    vertexLookup[vertexSets[*pBeginR]] = vertexLocation;
    vertexSets[*pBeginR] = vertex;
    vertexLookup[vertex] = *pBeginR;
    
    *pNewBeginR = *pBeginR;
    *pNewBeginP = *pBeginR;

    //printf("Before 1st while\n");
    // swap later neighbors of vertex into P section of vertexSets
    int j = 0;
    while(j<orderingArray[orderNumber]->laterDegree)
    {
        int neighbor = orderingArray[orderNumber]->later[j];
        int neighborLocation = vertexLookup[neighbor];

        (*pNewBeginP)--;

        vertexSets[neighborLocation] = vertexSets[*pNewBeginP];
        vertexLookup[vertexSets[*pNewBeginP]] = neighborLocation;
        vertexSets[*pNewBeginP] = neighbor;
        vertexLookup[neighbor] = *pNewBeginP;

        j++; 
    }

    *pNewBeginX = *pNewBeginP;

    // reset numNeighbors and neighborsInP for this vertex
    j = *pNewBeginP;
    //printf("Before 2nd while\n");
    while(j<*pNewBeginR)
    {
        int vertexInP = vertexSets[j];
        //printf("vertexInP = %d, numNeighbors[vertexInP]=%d\n", vertexInP, numNeighbors[vertexInP] );
        //printf("Address being freed: %p\n", neighborsInP[vertexInP]);
        numNeighbors[vertexInP] = 0;
        free(neighborsInP[vertexInP]);
        //printf("Allocating %d space for neighborsInP[vertexInP].\n", min( *pNewBeginR-*pNewBeginP, 
                                           //  orderingArray[vertexInP]->laterDegree 
                                           //+ orderingArray[vertexInP]->earlierDegree));
        neighborsInP[vertexInP]= (int *)calloc( std::min( *pNewBeginR-*pNewBeginP, 
                                             orderingArray[vertexInP]->laterDegree 
                                           + orderingArray[vertexInP]->earlierDegree), sizeof(int));

        j++;
    }

    // count neighbors in P, and fill in array of neighbors
    // in P
    j = *pNewBeginP;
    //printf("Before 3rd while\n");
    while(j<*pNewBeginR)
    {
        int vertexInP = vertexSets[j];

        int k = 0;
        while(k<orderingArray[vertexInP]->laterDegree)
        {
            int laterNeighbor = orderingArray[vertexInP]->later[k];
            int laterNeighborLocation = vertexLookup[laterNeighbor];

            if(laterNeighborLocation >= *pNewBeginP && laterNeighborLocation < *pNewBeginR)
            {
                neighborsInP[vertexInP][numNeighbors[vertexInP]] = laterNeighbor;
                numNeighbors[vertexInP]++;
                neighborsInP[laterNeighbor][numNeighbors[laterNeighbor]] = vertexInP;
                numNeighbors[laterNeighbor]++;
            }

            k++;
        }

        j++;
    }
}



/*! \brief Move a vertex to the set R, and update sets P and X
           and the arrays of neighbors in P

    \param vertex The vertex to move to R.

    \param vertexSets An array containing sets of vertices divided into sets X, P, R, and other.
 
    \param vertexLookup A lookup table indexed by vertex number, storing the index of that 
                        vertex in vertexSets.

    \param neighborsInP Maps vertices to arrays of neighbors such that 
                        neighbors in P fill the first cells

    \param numNeighbors An the neighbor of neighbors a vertex had in P,
                        the first time this function is called, this bound is 
                        used to keep us from allocating more than linear space.

    \param pBeginX The index where set X begins in vertexSets.
 
    \param pBeginP The index where set P begins in vertexSets.

    \param pBeginR The index where set R begins in vertexSets.

    \param pNewBeginX After function, contains the new index where set X begins
                      in vertexSets after adding vertex to R.
 
    \param pNewBeginP After function, contains the new index where set P begins
                      in vertexSets after adding vertex to P.

    \param pNewBeginR After function, contains the new index where set R begins
                      in vertexSets after adding vertex to R.
*/

void moveToRDegeneracyCliques( int vertex, 
                               int* vertexSets, int* vertexLookup, 
                               int** neighborsInP, int* numNeighbors,
                               int* , int *pBeginP, int *pBeginR, 
                               int* pNewBeginX, int* pNewBeginP, int *pNewBeginR)
{

    int vertexLocation = vertexLookup[vertex];
    
    (*pBeginR)--;
    vertexSets[vertexLocation] = vertexSets[*pBeginR];
    vertexLookup[vertexSets[*pBeginR]] = vertexLocation;
    vertexSets[*pBeginR] = vertex;
    vertexLookup[vertex] = *pBeginR;

    // this is not a typo, initially newX is empty
    *pNewBeginX = *pBeginP;
    *pNewBeginP = *pBeginP;
    *pNewBeginR = *pBeginP;

    int sizeOfP = *pBeginR - *pBeginP;

    int j = (*pBeginP);
    while(j<(*pBeginR))
    {
        int neighbor = vertexSets[j];
        int neighborLocation = j;

        int numPotentialNeighbors = std::min(sizeOfP, numNeighbors[neighbor]); 
        int k = 0;
        while(k<numPotentialNeighbors)
        {
            if(neighborsInP[neighbor][k] == vertex)
            {
                vertexSets[neighborLocation] = vertexSets[(*pNewBeginR)];
                vertexLookup[vertexSets[(*pNewBeginR)]] = neighborLocation;
                vertexSets[(*pNewBeginR)] = neighbor;
                vertexLookup[neighbor] = (*pNewBeginR);
                (*pNewBeginR)++;
            }

            k++;
        }

        j++;
    }

    j = (*pNewBeginP);

    while(j < *pNewBeginR)
    {
        int thisVertex = vertexSets[j];

        int numPotentialNeighbors = std::min(sizeOfP, numNeighbors[thisVertex]); 

        int numNeighborsInP = 0;

        int k = 0;
        while(k < numPotentialNeighbors)
        {
            int neighbor = neighborsInP[thisVertex][k];
            int neighborLocation = vertexLookup[neighbor];
            if(neighborLocation >= *pNewBeginP && neighborLocation < *pNewBeginR)
            {
                neighborsInP[thisVertex][k] = neighborsInP[thisVertex][numNeighborsInP];
                neighborsInP[thisVertex][numNeighborsInP] = neighbor;
                numNeighborsInP++;
            }
            k++;
        }

        j++;
    }
}

/*! \brief Move a vertex from the set R to the set X, and update all necessary pointers
           and arrays of neighbors in P

    \param vertex The vertex to move from R to X.

    \param vertexSets An array containing sets of vertices divided into sets X, P, R, and other.
 
    \param vertexLookup A lookup table indexed by vertex number, storing the index of that 
                        vertex in vertexSets.

    \param pBeginX The index where set X begins in vertexSets.
 
    \param pBeginP The index where set P begins in vertexSets.

    \param pBeginR The index where set R begins in vertexSets.

*/

void moveFromRToXDegeneracyCliques( int vertex, 
                                    int* vertexSets, int* vertexLookup, 
                                    int* , int* pBeginP, int* pBeginR )
{
    int vertexLocation = vertexLookup[vertex];

    //swap vertex into X and increment beginP and beginR
    vertexSets[vertexLocation] = vertexSets[*pBeginP];
    vertexLookup[vertexSets[*pBeginP]] = vertexLocation;
    vertexSets[*pBeginP] = vertex;
    vertexLookup[vertex] = *pBeginP;

    *pBeginP = *pBeginP + 1;
    *pBeginR = *pBeginR + 1;
}
