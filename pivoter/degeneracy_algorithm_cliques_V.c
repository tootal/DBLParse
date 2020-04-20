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
#include"MemoryManager.h"
#include"degeneracy_helper.h"
#include"degeneracy_algorithm_cliques_V.h"

extern double nCr[1001][401];


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


/*!

    \param adjList An array of linked lists, representing the input graph in the
                   "typical" adjacency list format.
 
    \param adjacencyList an array of arrays, representing the input graph in a more
                         compact and cache-friendly adjacency list format. (not currently used)

    \param cliques A linked list of cliques to return. <b>(only available when compiled 
                   with RETURN_CLIQUES_ONE_BY_ONE defined)</b>

    \param degree An array, indexed by vertex, containing the degree of that vertex. (not currently used)

    \param size The number of vertices in the graph.

    \return the number of maximal cliques of the input graph.
*/

void listAllCliquesDegeneracy_V( double * cliqueCounts, NeighborListArray** orderingArray, 
                                      int size, int max_k)
{

    // vertex sets are stored in an array like this:
    // |--X--|--P--|
    int* vertexSets = (int *)Calloc(size, sizeof(int));

    // vertex i is stored in vertexSets[vertexLookup[i]]
    int* vertexLookup = (int *)Calloc(size, sizeof(int));

    int** neighborsInP = (int **)Calloc(size, sizeof(int*));
    int* numNeighbors = (int *)Calloc(size, sizeof(int));
    
    int i = 0, deg = 0;

    while(i<size)
    {
        vertexLookup[i] = i;
        vertexSets[i] = i;
        neighborsInP[i] = (int *)Calloc(1, sizeof(int));
        numNeighbors[i] = 1;
        if ((int)orderingArray[i]->laterDegree > deg) deg = (int)orderingArray[i]->laterDegree;
        i++;
    }

    int beginX = 0;
    int beginP = 0;
    int beginR = size;

    int* dropV = (int *)Calloc(MAX_CSIZE, sizeof(int));
    int* keepV = (int *)Calloc(MAX_CSIZE, sizeof(int));

    // for each vertex
    for(i=0;i<size;i++)
    {
        int vertex = (int)orderingArray[i]->vertex;

        int newBeginX, newBeginP, newBeginR;

        // set P to be later neighbors and X to be be earlier neighbors
        // of vertex
        fillInPandXForRecursiveCallDegeneracyCliques( i, vertex, 
                                               vertexSets, vertexLookup, 
                                               orderingArray,
                                               neighborsInP, numNeighbors,
                                               &beginX, &beginP, &beginR, 
                                               &newBeginX, &newBeginP, &newBeginR);


        // recursively compute maximal cliques containing vertex, some of its
        // later neighbors, and avoiding earlier neighbors
        int drop = 0;
        int keep = 1;
        memset(keepV, 0, MAX_CSIZE);
        memset(dropV, 0, MAX_CSIZE);
        keepV[keep-1] = vertex;
        listAllCliquesDegeneracyRecursive_V(cliqueCounts,
                                                  vertexSets, vertexLookup,
                                                  neighborsInP, numNeighbors,
                                                  newBeginX, newBeginP, newBeginR, keep, drop, keepV, dropV, max_k); 

        beginR = beginR + 1;

    }
    
    Free(vertexSets);
    Free(vertexLookup);

    for(i = 0; i<size; i++)
    {
        Free(neighborsInP[i]);
        Free(orderingArray[i]->later);
        Free(orderingArray[i]->earlier);
        Free(orderingArray[i]);
    }

    Free(neighborsInP);
    Free(numNeighbors);

    return;
}

/*! \brief Recursively list all maximal cliques containing all of
           all vertices in R, some vertices in P and no vertices in X.

    \param cliqueCount A pointer to the number of maximal cliques computed 
                       thus far.

    \param cliques A linked list of cliques to return. <b>(only available when compiled 
                   with RETURN_CLIQUES_ONE_BY_ONE defined)</b>

    \param partialClique A linked list storing R, the partial clique for this
                         recursive call. 

    \param vertexSets An array containing sets of vertices divided into sets X, P, R and other.
 
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

void listAllCliquesDegeneracyRecursive_V( double * cliqueCounts,
                                               int* vertexSets, int* vertexLookup,
                                               int** neighborsInP, int* numNeighbors,
                                               int beginX, int beginP, int beginR, int keep, int drop, int *keepV, int *dropV, int max_k)
{
    if ((beginP >= beginR) || (keep > max_k))
    {
        double kkeepCliques = 0; // number of kcliques a vertex from "keep" is involved in
        for (int i=drop; (i>=0) && (keep+drop-i <= max_k); i--) 
        {
            int k = keep + drop - i;
            kkeepCliques = nCr[drop][i];
            for (int j=0; j<keep; j++)
            {
                int v = keepV[j];
                cliqueCounts[v*(max_k+1)+k] += kkeepCliques;
            }
        }
        double kdropCliques = 0;
        for (int i=drop-1; (i>=0) && (keep+drop-i <= max_k); i--) 
        {
            int k = keep + drop - i;
            kdropCliques = nCr[drop-1][i];
            for (int j=0; j<drop; j++)
            {
                int v = dropV[j];
                cliqueCounts[v*(max_k+1)+k] += kdropCliques;
            }
        }
        return;
    }
    
    int* myCandidatesToIterateThrough;
    int numCandidatesToIterateThrough;

    // get the candidates to add to R to make a maximal clique
    int pivot = findBestPivotNonNeighborsDegeneracyCliques( &myCandidatesToIterateThrough,
                                         &numCandidatesToIterateThrough,
                                         vertexSets, vertexLookup,
                                         neighborsInP, numNeighbors,
                                         beginX, beginP, beginR);

    

    // add candiate vertices to the partial clique one at a time and 
    // search for maximal cliques
    if(numCandidatesToIterateThrough != 0)
    {
        int iterator = 0;
        while(iterator < numCandidatesToIterateThrough)
        {
            // vertex to be added to the partial clique
            int vertex = myCandidatesToIterateThrough[iterator];

            int newBeginX, newBeginP, newBeginR;

            // swap vertex into R and update all data structures 
            moveToRDegeneracyCliques( vertex, 
                               vertexSets, vertexLookup, 
                               neighborsInP, numNeighbors,
                               &beginX, &beginP, &beginR, 
                               &newBeginX, &newBeginP, &newBeginR);


            
            // recursively compute maximal cliques with new sets R, P and X
            if (vertex == pivot)
            {
                dropV[drop] = vertex;
                listAllCliquesDegeneracyRecursive_V(cliqueCounts,
                                                      vertexSets, vertexLookup,
                                                      neighborsInP, numNeighbors,
                                                      newBeginX, newBeginP, newBeginR, keep, drop+1, keepV, dropV, max_k);
            }
            else
            {
                keepV[keep] = vertex;
                listAllCliquesDegeneracyRecursive_V(cliqueCounts,
                                                      vertexSets, vertexLookup,
                                                      neighborsInP, numNeighbors,
                                                      newBeginX, newBeginP, newBeginR, keep+1, drop, keepV, dropV, max_k);
            }

            moveFromRToXDegeneracyCliques( vertex, 
                                    vertexSets, vertexLookup,
                                    &beginX, &beginP, &beginR );

            iterator++;
        }

        // swap vertices that were moved to X back into P, for higher recursive calls.
        iterator = 0;
        while(iterator < numCandidatesToIterateThrough)
        {
            int vertex = myCandidatesToIterateThrough[iterator];
            int vertexLocation = vertexLookup[vertex];

            beginP--;
            vertexSets[vertexLocation] = vertexSets[beginP];
            vertexSets[beginP] = vertex;
            vertexLookup[vertex] = beginP;
            vertexLookup[vertexSets[vertexLocation]] = vertexLocation;

            iterator++;
        }
    }

    // don't need to check for emptiness before freeing, since
    // something will always be there (we allocated enough memory
    // for all of P, which is nonempty)
    Free(myCandidatesToIterateThrough);

    return;
}
