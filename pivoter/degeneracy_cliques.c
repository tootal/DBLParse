#include "LinkedList.h"
#include "MemoryManager.h"
#include "misc.h"

int degeneracyCliques(char *fname) {
    // number of vertices
    int n;
    
    // 2x number of edges
    int m;
    
    // type: A/V/E. 
    // A for just k-clique information, 
    // V for per-vertex k-cliques, 
    // E for per-edge k-cliques
    char t = 'A'; 
    
    // max_clique_size: max_clique_size. 
    // If 0, calculate for all k.
    int max_k = 0; 
    
    // data_flag: 1 if information is to be output to a file, 0 otherwise.
    int flag_d = 0;
    
    LinkedList** adjacencyList = readInGraphAdjListToDoubleEdges(&n, &m, fname);
    populate_nCr();
    runAndPrintStatsCliques(adjacencyList, n, fname, t, max_k, flag_d);
    
    int i = 0;
    while (i<n) {
        destroyLinkedList(adjacencyList[i]);
        i++;
    }
    Free(adjacencyList); 
    return 0;
}
