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
#include <QVector>

#include"misc.h"
#include"degeneracy_helper.h"

/*! \brief

    \param list an input graph, represented as an array of linked lists of integers

    \param size the number of vertices in the graph

    \return an array of NeighborListArrays representing a degeneracy ordering of the vertices.

    \see NeighborListArray
*/

void computeDegeneracyOrderArray(QVector<QVector<int>> &list,
                                 int size,
                                 QVector<NeighborListArray*> &orderingArray)
{
    QVector<NeighborList*> ordering(size);
    
    int i = 0;

    int degeneracy = 0;
    
    // array of lists of vertices, indexed by degree
    QVector<std::list<int>> verticesByDegree(size);
    
    // array of lists of vertices, indexed by degree
    QVector<std::list<int>::iterator> vertexLocator(size);
    
    QVector<int> degree(size);
    
    for(i = 0; i < size; i++) {
        ordering[i] = new NeighborList;
    }

    // fill each cell of degree lookup table
    // then use that degree to populate the 
    // lists of vertices indexed by degree
    for (i = 0; i < size; i++) {
        degree[i] = list[i].size();
        verticesByDegree[degree[i]].push_front(i);
        vertexLocator[i] = verticesByDegree[degree[i]].begin();
    }
    int currentDegree = 0;

    int numVerticesRemoved = 0;

    while (numVerticesRemoved < size) {
        if (!verticesByDegree[currentDegree].empty()) {
            degeneracy = std::max(degeneracy,currentDegree);
            
            int vertex = verticesByDegree[currentDegree].front();
            verticesByDegree[degree[vertex]].erase(vertexLocator[vertex]);

            ordering[vertex]->vertex = vertex;
            ordering[vertex]->orderNumber = numVerticesRemoved;
            degree[vertex] = -1;

            for (int neighbor : list[vertex]) {
                if (degree[neighbor]!=-1) {
                    verticesByDegree[degree[neighbor]].erase(vertexLocator[neighbor]);
                    ordering[vertex]->later.push_back(neighbor);
                    degree[neighbor]--;

                    if(degree[neighbor] != -1) {
                        verticesByDegree[degree[neighbor]].push_front(neighbor);
                        vertexLocator[neighbor] = verticesByDegree[degree[neighbor]].begin();
                    }
                } else {
                    ordering[vertex]->earlier.push_back(neighbor);
                }
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
        orderingArray[i]->laterDegree = ordering[i]->later.size();
        orderingArray[i]->later.resize(orderingArray[i]->laterDegree);

        int j=0;
        auto curr = ordering[i]->later.begin();
        while (curr != ordering[i]->later.end()) {
            orderingArray[i]->later[j++] = *curr;
            curr++;
        }

        orderingArray[i]->earlierDegree = ordering[i]->earlier.size();
        orderingArray[i]->earlier.resize(orderingArray[i]->earlierDegree);

        j=0;
        curr = ordering[i]->earlier.begin();
        while (curr != ordering[i]->earlier.end()) {
            orderingArray[i]->earlier[j++] = *curr;
            curr++;
        }
    }
    for(i = 0; i < size; i++) {
        ordering[i]->earlier.clear();
        ordering[i]->later.clear();
        delete ordering[i];
        verticesByDegree[i].clear();
    }
}
