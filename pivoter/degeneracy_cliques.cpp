#include "LinkedList.h"
#include "misc.h"
#include "degeneracy_cliques.h"

int degeneracyCliques() {
    // number of vertices
    int n;
    
    // 2x number of edges
    int m;
    
    // max_clique_size: max_clique_size. 
    // If 0, calculate for all k.
    int max_k = 0; 
    
    LinkedList** adjacencyList = readInGraphAdjListToDoubleEdges(&n, &m);
    populate_nCr();
    runAndPrintStatsCliques(adjacencyList, n, 'A', max_k);
    
    int i = 0;
    while (i<n) {
        destroyLinkedList(adjacencyList[i]);
        i++;
    }
    free(adjacencyList); 
    return 0;
}
