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
#include"degeneracy_algorithm_cliques_E.h"

extern double nCr[1001][401];


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

void listAllCliquesDegeneracy_E(double* cliqueCounts, 
                                      NeighborListArray** orderingArray,
                                      int *ordering,
                                      int *CSCindex,
                                      int *CSCedges,
                                      int size, int max_k)
{
    // vertex sets are stored in an array like this:
    // |--X--|--P--|
    int* vertexSets = (int *)Calloc(size, sizeof(int));

    // vertex i is stored in vertexSets[vertexLookup[i]]
    int* vertexLookup = (int *)Calloc(size, sizeof(int));

    int** neighborsInP = (int **)Calloc(size, sizeof(int*));
    int* numNeighbors = (int *)Calloc(size, sizeof(int));

    int i = 0;
    int deg = 0;

    while(i<size)
    {
        vertexLookup[i] = i;
        vertexSets[i] = i;
        neighborsInP[i] = (int *)Calloc(1, sizeof(int));
        numNeighbors[i] = 1;
        i++;
    }
    
    int beginX = 0;
    int beginP = 0;
    int beginR = size;

    int* dropV = (int *)Calloc(MAX_CSIZE, sizeof(int));
    int* keepV = (int *)Calloc(MAX_CSIZE, sizeof(int));
  //printf("Ready?");
    //scanf("%d", &i);
    // for each vertex
    for(i=0;i<size;i++)
    {
        int vertex = (int)orderingArray[i]->vertex;
        //printf("vertex = %d", vertex);
        //fflush(stdout);
        int newBeginX, newBeginP, newBeginR;

        //printf("Before fillIn\n");
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
        //printf("Calling Rec function.\n");
        listAllCliquesDegeneracyRecursive_E(cliqueCounts,
                                                  ordering,
                                                  CSCindex,
                                                  CSCedges,
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

void listAllCliquesDegeneracyRecursive_E(double* cliqueCounts,
                                               int *ordering,
                                               int *CSCindex,
                                               int *CSCedges,
                                               int* vertexSets, int* vertexLookup,
                                               int** neighborsInP, int* numNeighbors,
                                               int beginX, int beginP, int beginR,
                                               int keep, int drop, int *keepV, int *dropV, int max_k)
{
  //printf("In rec function.\n");
    if ((beginP >= beginR) || (keep > max_k))
    {
        int temp = 0;
        for (int i=0; i<keep; i++)
        {
            for (int j=i+1; j<keep; j++)
            {
                int u = keepV[i];
                int v = keepV[j];
                if (ordering[u] > ordering[v])
                {
                    temp = u;
                    u = v;
                    v = temp;
                }

                int index = findNbrCSC(u,v,CSCindex, CSCedges); // find the index of v in u's neighbor list

                if (index == -1) fprintf(stderr, "Error. v not found in u's list\n");

                for (int l=drop; (l>=0) && (keep+drop-l <= max_k); l--) 
                {
                    int k = keep + drop - l;
                    long in = (long)index*(long)(max_k+1) + (long)k;
                    cliqueCounts[in] += nCr[drop][l];
                }
            }
        }
        for (int i=0; i<drop; i++)
        {
            for (int j=i+1; j<drop; j++)
            {
                int u = dropV[i];
                int v = dropV[j];
                if (ordering[u] > ordering[v])
                {
                    temp = u;
                    u = v;
                    v = temp;
                }
                int index = findNbrCSC(u,v,CSCindex, CSCedges); // find the index of v in u's neighbor list
                if (index == -1) fprintf(stderr, "Error. v not found in u's list\n");
                for (int l=drop-2; (l>=0) && (keep+drop-l <= max_k); l--)
                {
                    int k = keep + drop - l;
                    long in = (long)index*(long)(max_k+1) + (long)k;
                    cliqueCounts[in] += nCr[drop-2][l];
                }
            }
        }

        for (int i=0; i<keep; i++)
        {
            for (int j=0; j<drop; j++)
            {
                int u = keepV[i];
                int v = dropV[j];
                if (ordering[u] > ordering[v])
                {
                    temp = u;
                    u = v;
                    v = temp;
                }
                int index = findNbrCSC(u,v,CSCindex, CSCedges); // find the index of v in u's neighbor list
                if (index == -1) fprintf(stderr, "Error. v not found in u's list\n");
                for (int l=drop-1; (l>=0) && (keep+drop-l <= max_k); l--)
                {
                    int k = keep + drop - l;
                    long s = (long)(max_k + 1);
                    long in = (long)index*s + (long)k;
                    cliqueCounts[in] += nCr[drop-1][l];
                }
            }
        }
        return;
    }
    
    int* myCandidatesToIterateThrough;
    int numCandidatesToIterateThrough;

    //printf("About to find pivot\n");
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

            // add vertex into partialClique, representing R.
            // Link* vertexLink = addLast(partialClique, (int)vertex);

            //printf("About to call moveToR\n");
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
                listAllCliquesDegeneracyRecursive_E(cliqueCounts,
                                                      ordering,
                                                      CSCindex,
                                                      CSCedges,
                                                      vertexSets, vertexLookup,
                                                      neighborsInP, numNeighbors,
                                                      newBeginX, newBeginP, newBeginR, keep, drop+1, keepV, dropV, max_k);
            }
            else
            {
                keepV[keep] = vertex;
                listAllCliquesDegeneracyRecursive_E(cliqueCounts,
                                                      ordering,
                                                      CSCindex,
                                                      CSCedges,
                                                      vertexSets, vertexLookup,
                                                      neighborsInP, numNeighbors,
                                                      newBeginX, newBeginP, newBeginR, keep+1, drop, keepV, dropV, max_k);
            }
  //printf("Returned from rec function.  Calling moveFromR.\n");
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
