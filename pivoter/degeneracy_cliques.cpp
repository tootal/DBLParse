#include "LinkedList.h"
#include "misc.h"
#include "degeneracy_cliques.h"

int degeneracyCliques() {
    // number of vertices
    int n;
    // 2x number of edges
    int m;
    FILE *fp;
    fp = fopen ("authors.edges", "r");
    fscanf(fp, "%d %d", &n, &m);
    LinkedList** adjList = (LinkedList**)calloc(n, sizeof(LinkedList*));
    for (int i = 0; i < n; i++)
        adjList[i] = createLinkedList();
    for (int i = 0; i < m; i++) {
        int u, v;
        fscanf(fp, "%d %d\n", &u, &v);
        addLast(adjList[u], v);
        addLast(adjList[v], u);
    }
    m = m * 2;
    fclose(fp);
    populate_nCr();
    runAndPrintStatsCliques(adjList, n);
    for (int i = 0; i < n; i++)
        destroyLinkedList(adjList[i]);
    free(adjList); 
    return 0;
}
