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

#pragma once

#include <algorithm>

#include <QVector>

#include "LinkedList.h"
#include "degeneracy_helper.h"
#include "degeneracy_algorithm_cliques_A.h"

extern QVector<QVector<BigInteger>> nCr;

void populate_nCr();

void runAndPrintStatsCliques(QVector<LinkedList*> &adjListLinked, 
                             int n);

void moveFromRToXDegeneracyCliques(int vertex,
                                   QVector<int> &vertexSets,
                                   QVector<int> &vertexLookup,
                                   int *pBeginX,
                                   int *pBeginP,
                                   int *pBeginR);

void moveToRDegeneracyCliques(int vertex, 
                              QVector<int> &vertexSets,
                              QVector<int> &vertexLookup, 
                              QVector<QVector<int>> &neighborsInP,
                              QVector<int> &numNeighbors,
                              int *pBeginX,
                              int *pBeginP,
                              int *pBeginR, 
                              int *pNewBeginX,
                              int *pNewBeginP,
                              int *pNewBeginR);

void fillInPandXForRecursiveCallDegeneracyCliques(int vertex,
                                                  int orderNumber,
                                                  QVector<int> &vertexSets,
                                                  QVector<int> &vertexLookup,
                                                  QVector<NeighborListArray*> &orderingArray,
                                                  QVector<QVector<int>> &neighborsInP,
                                                  QVector<int> &numNeighbors,
                                                  int *pBeginX,
                                                  int *pBeginP,
                                                  int *pBeginR, 
                                                  int *pNewBeginX,
                                                  int *pNewBeginP,
                                                  int *pNewBeginR);

int findBestPivotNonNeighborsDegeneracyCliques(QVector<int> &pivotNonNeighbors,
                                               int *numNonNeighbors,
                                               QVector<int> &vertexSets,
                                               QVector<int> &vertexLookup,
                                               QVector<QVector<int>> &neighborsInP,
                                               QVector<int> &numNeighbors,
                                               int beginX,
                                               int beginP,
                                               int beginR);
