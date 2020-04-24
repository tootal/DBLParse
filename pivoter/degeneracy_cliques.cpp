#include "LinkedList.h"
#include "misc.h"
#include "degeneracy_cliques.h"

int degeneracyCliques() {
    // number of vertices
    int n;
    
    // 2x number of edges
    int m;
    
    LinkedList** adjList = readInGraphAdjListToDoubleEdges(&n, &m);
    populate_nCr();
    runAndPrintStatsCliques(adjList, n);
    
    int i = 0;
    while (i<n) {
        destroyLinkedList(adjList[i]);
        i++;
    }
    free(adjList); 
    return 0;
}
