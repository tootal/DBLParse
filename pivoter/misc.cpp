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
#include"MemoryManager.h"
#include"degeneracy_helper.h"


BigNumber nCr[1001][401];

void populate_nCr()
{
    /*
    FILE *infile;
    infile = fopen("nCr.txt","r");
    */
//    double d=0;
    /*
    if(infile==NULL)
    {
        printf("file could not be opened\n");
        exit(1);
    }
    */

    for(int row = 0; row < 1001; ++row)
    {
        for (int col = 0; col < 401; ++col)
        {
//            if (!fscanf(infile,"%lf,",&d)) 
//                fprintf(stderr, "Error\n");
//             fprintf(stderr, "%lf\n", d);
//            nCr[row][col] = d;
            if (row == 0 || col == 0 || row == col) nCr[row][col] = 1;
            else nCr[row][col] = nCr[row-1][col] + nCr[row-1][col-1];
        }
    }
//    fclose(infile);
}

/*! \brief compare integers return -1,0,1 for <,=,>

    \param node1 an integer

    \param node2 an integer

    \return -1 if <, 0 if =, and 1 if >.
*/

int nodeComparator(int node1, int node2)
{
    if ((int)(size_t)node1 < (int)(size_t)node2)
        return -1;
    if((int)(size_t)node1 > (int)(size_t)node2)
        return 1;

    return 0;
}

/*! \brief compare integer pointers; return -1,0,1 for <,=,>;
           used for calling sort().

    \param node1 a pointer to an integer

    \param node2 a pointer to an integer

    \return -1 if <, 0 if =, and 1 if >.
*/

int sortComparator(int node1, int node2)
{
    if (*(int*)node1 < *(int*)node2)
        return -1;
    if(*(int*)node1 > *(int*)node2)
        return 1;

    return 0;
}

int qsortComparator(const void * node1, const void * node2)
{
    return ( *(int*)node1 - *(int*)node2 );
}

/*! \brief print an array of integers to standard out.

    \param array the array to print

    \param size the length of the array
*/

void printArray(int* array, int size)
{
    int i = 0;
    while(i<size)
        printf("%d ", array[i++]);
    printf("\n");
}

/*! \brief print an abbreviated version of an adjacency list

    \param listOfLists the adjacency list

    \param size the number of vertices in the graph
*/

void printArrayOfLinkedLists(LinkedList** listOfLists, int size)
{
    // list graph contents

    int i=0;

    while(i<size)
    {
        if(!isEmpty(listOfLists[i]))
        {
            printf("%d:", i);
            // printListAbbv(listOfLists[i], &printInt);
        }
        i++;
    }
}

/*! \brief print a clique, that is formatted as an integer
           array ending with -1.

    \param clique the clique.
*/

void printClique(int* clique)
{
    int i = 0;
    while(clique[i]!=-1)
    {
        printf("%d", clique[i]);
        if(clique[i+1]!=-1)
            printf(" ");
        i++;
    }
    printf("\n");
}

/*! \brief print an integer 

    \param integer an integer cast to a int
*/

void printInt(int integer)
{
    printf("%d", (int)(size_t)integer);
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

#ifdef DEBUG
        int i=0;
        while(clique[i] != -1)
        {
            printf("%d", clique[i]);
            if(clique[i+1] != -1)
                printf(" ");
            i++;
        }
        printf("\n");
#endif
        Free(clique);
        curr = curr->next;
    } 

    destroyLinkedList(cliques); 
}

/*! \brief read in a graph from stdin and return an 
           adjacency list, as an array of linked lists
           of integers.

    \param n this will be the number of vertices in the
             graph when this function returns.

    \param m this will be 2x the number of edges in the
             graph when this function returns.

    \return an array of linked lists of integers (adjacency list) 
            representation of the graph
*/

LinkedList** readInGraphAdjList(int* n, int* m)
{
    int u, v; // endvertices, to read edges.

    if(scanf("%d", n)!=1)
    {
        fprintf(stderr, "problem with line 1 in input file\n");
        exit(1);
    }

    if(scanf("%d", m)!=1)
    {
        fprintf(stderr, "problem with line 2 in input file\n");
        exit(1);
    }

#ifdef DEBUG
    printf("Number of vertices: %d\n", *n);
    printf("Number of edges: %d\n", *m);
#endif
    
    LinkedList** adjList = (LinkedList**)Calloc(*n, sizeof(LinkedList*));

    int i = 0;
    while(i < *n)
    {
        adjList[i] = createLinkedList();
        i++;
    }

    i = 0;

    while(i < *m)
    {
        if(scanf("%d,%d", &u, &v)!=2)
        {
            printf("problem with line %d in input file\n", i+2);
            exit(1);
        }
        assert(u < *n && u > -1);
        assert(v < *n && v > -1);
        if(u==v)
            printf("%d=%d\n", u, v);
        assert(u != v);

        addLast(adjList[u], (int)v);
        // addLast(adjList[v], (int)u);

        i++;
    }

#ifdef DEBUG
    printArrayOfLinkedLists(adjList, *n);
#endif

    return adjList;
}

/*! \brief read in a graph from stdin and return an 
           adjacency list, as an array of linked lists
           of integers.

    \param n this will be the number of vertices in the
             graph when this function returns.

    \param m this will be 2x the number of edges in the
             graph when this function returns.

    \return an array of linked lists of integers (adjacency list) 
            representation of the graph
*/

LinkedList** readInGraphAdjListToDoubleEdges(int* n, int* m, char *)
{
    int u, v; // endvertices, to read edges.

    FILE *fp;
    fp = fopen ("authors.edges", "r");
    if (!fp) 
    {
        fprintf(stderr, "Could not open input file.\n");
        exit(1);
    }

    if(fscanf(fp, "%d %d", n, m)!=2)
    {
        fprintf(stderr, "Number of vertices: %d\n", *n);
        fprintf(stderr, "Number of edges: %d\n", *m);
        fprintf(stderr, "problem with line 1 in input file\n");
        exit(1);
    }

    LinkedList** adjList = (LinkedList**)Calloc(*n, sizeof(LinkedList*));

    int i = 0;
    while(i < *n)
    {
        adjList[i] = createLinkedList();
        i++;
    }

    i = 0;
    // double maxv = 0;
    while(i < *m)
    {
        if (fscanf(fp, "%d %d\n", &u, &v)!=2)
        {
            printf("problem with line %d in input file, u=%d, v=%d\n", i+2, u, v);
            exit(1);
        }
        // if ((u>= *n) || (v >= *n)) printf("u = %d, v = %d \n", u, v);
        // if ((double) u > maxv) maxv = (double) u;
        // if ((double) v > maxv) maxv = (double) v;
        assert(u < *n && u > -1);
        assert(v < *n && v > -1);
        assert(u != v);

        addLast(adjList[u], (int)v);
        addLast(adjList[v], (int)u);
        
        i++;
    }
    // printf("maxv = %lf \n", maxv);
    *m = (*m) * 2;

    fclose(fp);
    return adjList;
}

void runAndPrintStatsCliques(  LinkedList** adjListLinked,
                               int n, const char *, 
                               char T, int max_k, int flag_d)
{
  //printf("In runAndPrint function.\n");
    fflush(stderr);
    int max_k_in = max_k;

    clock_t start = clock();
    
//    double totalCliques = 0;
    BigNumber totalCliques = 0;
    int deg = 0, m = 0;
    FILE *fp;

    //printf("Before if of flag_d.\n");
    fflush(stdout);
    /*if ((flag_d == 1) || (flag_d == 2))
    {
        
        char *fname = (char *)Calloc(1000, sizeof(char));

        strcpy(fname, "results/");
        strcat(fname, gname);
        char * s_max_k = (char *)Calloc(10,sizeof(char));
        sprintf(s_max_k, "%d", max_k);
        if (max_k > 0) {strcat(fname, "_"); strcat(fname, s_max_k);}
        if (T == 'A')
        {
            if (flag_d == 2) strcat(fname, "_A_stat.txt");
            else strcat(fname, "_A.txt");
        }
        else if (T == 'V') 
        {
            if (flag_d == 2) strcat(fname, "_V_stat.txt");
            else strcat(fname, "_V.txt");
        }
        else 
        {
            if (flag_d == 2) strcat(fname, "_E_stat.txt");
            else strcat(fname, "_E.txt");
        }

        fp = fopen (fname,"w");
        if (!fp) printf("Could not open output file.\n");
    }*/
    fp = fopen ("authors_cliques.txt", "w");
    if (!fp) printf("Could not open output file.\n");
    //printf("Before computeDegeneracy.\n");
    fflush(stdout);

    NeighborListArray** orderingArray = computeDegeneracyOrderArray(adjListLinked, n);
    //printf("Before for. After computeDegeneracy.\n");
    fflush(stdout);
    for (int i=0; i<n; i++)
    {
        if (deg < orderingArray[i]->laterDegree) deg = orderingArray[i]->laterDegree;
        m += orderingArray[i]->laterDegree;
    }

    if (max_k == 0) max_k = deg + 1;

    if (T == 'A')
    {

        double *nCalls = (double *)Calloc(1, sizeof(double));
        double *sumP = (double *)Calloc(1, sizeof(double));
        double *sqP = (double *)Calloc(1, sizeof(double));
        std::vector<BigNumber> cliqueCounts(max_k + 1);
//        double *cliqueCounts = (double *) Calloc((max_k)+1, sizeof(double));
        listAllCliquesDegeneracy_A(cliqueCounts, orderingArray, n, max_k, nCalls, sumP, sqP);
        clock_t end = clock();
       
//        printf("time,nCalls,sumP,sqP,max_k,degen\n");
//        printf("%lf,%lf,%lf,%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),*nCalls,*sumP,*sqP,max_k_in,deg);
        /*if ((flag_d == 1) || (flag_d == 2))
        {
            fprintf(fp, "time,nCalls,sumP,sqP,max_k,degen\n");
            fprintf(fp, "%lf,%lf,%lf,%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),*nCalls,*sumP,*sqP,max_k_in,deg);
        }

        if (flag_d == 1) fprintf(fp, "k, Ck\n");
        */
        while (cliqueCounts[max_k] == 0) max_k--;
        fprintf(fp, "%d\n", max_k);
        for (int i=1; i<=max_k; i++)
        {
            fprintf(fp, "%d %s\n", i, cliqueCounts[i].getString().c_str()); 
//            if (flag_d == 1) fprintf(fp, "%d, %lf\n", i, cliqueCounts[i]); 
            totalCliques += cliqueCounts[i];
        }

        fprintf(fp, "%s\n", totalCliques.getString().c_str());
//        if (flag_d == 1) fprintf(fp, "\n%lf total cliques\n", totalCliques);

//        Free(cliqueCounts);
    }
    /*else if (T == 'V')
    {
        double *cliqueCounts = (double *) Calloc(n*((max_k)+1), sizeof(double));
        listAllCliquesDegeneracy_V(cliqueCounts, orderingArray, n, max_k);
        clock_t end = clock();

        BigNumber kcliques = 0;

        printf("time,max_k,degen\n");
        printf("%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),max_k_in, deg);
        if ((flag_d == 1) || (flag_d == 2))
        {
            fprintf(fp,"time,max_k,degen\n");
            fprintf(fp,"%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),max_k_in, deg);
        }

        if (flag_d == 1) fprintf(fp, "(v,k): Ck\n");

        for (int j=1; j<=max_k; j++)
        {
            kcliques = 0;
            for (int i=0; i<n; i++)
            {
                kcliques += cliqueCounts[(i*(max_k+1)) + j];
                if ((cliqueCounts[(i*(max_k+1)) + j] != 0) && (flag_d == 1))fprintf(fp,"(%d, %d): %.0lf\n", i, j, cliqueCounts[(i*(max_k+1)) + j]); 
            }

            if (!(kcliques == BigNumber(0)))
            {
                printf("%d, %s\n", j, (kcliques/BigNumber(j)); 
                totalCliques += ((double)(kcliques)/(double)(j));
            }
        }
        if (flag_d == 1) fprintf(fp,"\n%s total cliques\n", totalCliques.getString().c_str());

        printf("\n%s total cliques\n", totalCliques.getString().c_str());
        
        Free(cliqueCounts);
    }
    else if (T == 'E')
    {
        int* ordering = (int *)Calloc(n, sizeof(int));
        int* CSCindex = (int *)Calloc(n+1, sizeof(int));
        int* CSCedges = (int *)Calloc(m, sizeof(int));

        double *cliqueCounts = (double *)Calloc(m*(max_k+1), sizeof(double));

        int index = 0;
        int degen = deg;

        for (int i=0; i<n; i++)
        {
            ordering[orderingArray[i]->vertex] = (int)orderingArray[i]->orderNumber;
            deg = orderingArray[i]->laterDegree;
            CSCindex[i] = index;
            memcpy(CSCedges+index, orderingArray[i]->later, (deg)*sizeof(int));
            qsort(CSCedges+index, deg, sizeof(int), qsortComparator);
            index += deg;   
        }

        CSCindex[n] = m;

        listAllCliquesDegeneracy_E(cliqueCounts, 
                                    orderingArray,
                                    ordering,
                                    CSCindex,
                                    CSCedges,
                                    n, max_k);

        clock_t end = clock();

        printf("time,max_k,degen\n");
        printf("%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),max_k_in, degen);
        if ((flag_d == 1) || (flag_d == 2))
        {
            fprintf(fp,"time,max_k,degen\n");
            fprintf(fp,"%lf,%d,%d\n\n", (double)(end-start)/(double)(CLOCKS_PER_SEC),max_k_in, degen);
        }
    
        if (flag_d == 1) fprintf(fp, "(u,v,k): Ck\n");
        double *kcliques = (double *)Calloc(max_k+1, sizeof(double));

        for (long i=0; i<n; i++)
        {
            for (long j=CSCindex[i]; j<CSCindex[i+1]; j++)
            {
                for (long k=2; k<=max_k; k++)
                {
                    if (cliqueCounts[((j*(max_k+1)) + k)] != 0) 
                    {
                        kcliques[k] += cliqueCounts[((j*(max_k+1)) + k)];
                        if (flag_d == 1) fprintf (fp, "(%d, %d, %d): %.0lf\n", i, CSCedges[j], k, cliqueCounts[((j*(max_k+1)) + k)]);
                    }
                }
            }
        }

        for (int k=2; k<=max_k; k++)
        {
            if (kcliques[k] != 0) 
            {
                printf("%d, %lf\n", k, 2*kcliques[k]/((double)k*((double)k-1))); 
                totalCliques += 2*kcliques[k]/((double)k*((double)k-1));
            }
        }
        
        printf("\n%lf total cliques\n", totalCliques);
        if (flag_d == 1) fprintf(fp,"\n%lf total cliques\n", totalCliques);

        Free(ordering);
        Free(CSCindex);
        Free(CSCedges);
        Free(kcliques);
    }
    */
    fclose(fp);
    if (flag_d >= 1) fclose(fp);
    Free(orderingArray);
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
                                                int beginX, int beginP, int beginR)
{
    int pivot = -1;
    int maxIntersectionSize = -1;

    // iterate over each vertex in P union X 
    // to find the vertex with the most neighbors in P.
    int j = beginP;
    while(j<beginR)
    {
        int vertex = vertexSets[j];
        int numPotentialNeighbors = min(beginR - beginP, numNeighbors[vertex]); //bug resolved by Shweta

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
    *pivotNonNeighbors = (int *)Calloc(beginR-beginP, sizeof(int));
    memcpy(*pivotNonNeighbors, &vertexSets[beginP], (beginR-beginP)*sizeof(int));

    // we will decrement numNonNeighbors as we find neighbors
    *numNonNeighbors = beginR-beginP;

    int numPivotNeighbors = min(beginR - beginP, numNeighbors[pivot]); //bug resolved by Shweta
  
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
                                                   int* pBeginX, int *pBeginP, int *pBeginR, 
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
        Free(neighborsInP[vertexInP]);
        //printf("Allocating %d space for neighborsInP[vertexInP].\n", min( *pNewBeginR-*pNewBeginP, 
                                           //  orderingArray[vertexInP]->laterDegree 
                                           //+ orderingArray[vertexInP]->earlierDegree));
        neighborsInP[vertexInP]= (int *)Calloc( min( *pNewBeginR-*pNewBeginP, 
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
                               int* pBeginX, int *pBeginP, int *pBeginR, 
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

        int numPotentialNeighbors = min(sizeOfP, numNeighbors[neighbor]); 
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

        int numPotentialNeighbors = min(sizeOfP, numNeighbors[thisVertex]); 

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
                                    int* pBeginX, int* pBeginP, int* pBeginR )
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

int findNbrCSC(int u, int v, int *CSCindex, int *CSCedges)
{
    int index = -1;

    int first = CSCindex[u], last = CSCindex[u+1] - 1;
    int middle = (first+last)/2;

    while (first <= last) 
    {
        if (CSCedges[middle] < v)
            first = middle + 1;    
        else if (CSCedges[middle] == v) 
        {
            index = middle;
            break;
        }
        else
            last = middle - 1;
 
        middle = (first + last)/2;
    }
    
    return index;
}
