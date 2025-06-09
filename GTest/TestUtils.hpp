#pragma once

#include <iostream>
#include <vector>

#include <gtest/gtest.h>
#include "Utils.hpp"

using namespace PolyhedraLibrary;
using namespace std;
using namespace Eigen;

TEST(TestUtils, TestFindBarycenter)
{
    
}

TEST(TestUtils, TestBFS)
{
    vector<vector<int>> adjList = { {1, 2},     // Nodo 0 
                                    {2},        // Nodo 1 
                                    {0, 3, 4},  // Nodo 2 
                                    {4},        // Nodo 3
                                    {3, 5},     // Nodo 4
                                    {4, 3} };   // Nodo 5  
    int v1 = 2;
    int v2 = 5;
    int numVert = 6;
    double lengthEdge = 1.0;
    double lengthPath = 0.0;
    vector<int> minPathBFS = BFS(adjList, v1, v2, numVert, lengthEdge, lengthPath);
    vector<int> minPathEx = {2, 4, 5};
    EXPECT_EQ(minPathBFS, minPathEx);
}

TEST(TestUtils, TestDijkstra)
{
    vector<vector<int>> adjList = { {1, 2, 3, 4},   // Nodo 0 
                                    {0},            // Nodo 1 
                                    {1, 3, 4},      // Nodo 2 
                                    {2, 4},         // Nodo 3
                                    {0, 1} };       // Nodo 4
    MatrixXd matrWeights(5, 5); 
    matrWeights << 0, 5, 2, 3, 4,
                   5, 0, 0, 0, 0,
                   0, 2, 0, 2, 1,
                   0, 0, 1, 0, 2,
                   4, 1, 0, 0, 0; 
    int v1 = 3;
    int v2 = 0;
    int numVert = 5;
    double lengthPath = 0.0;
    vector<int> minPathDijkstra = Dijkstra(adjList, v1, v2, numVert, matrWeights, lengthPath);
    vector<int> minPathEx = {3, 4, 0};
    EXPECT_EQ(minPathDijkstra, minPathEx);
    EXPECT_DOUBLE_EQ(lengthPath, 6.0);

    vector<vector<int>> adjList1 = { {1, 2},        // Nodo 0 
                                    {0, 2, 3, 4},   // Nodo 1 
                                    {0, 1, 4},      // Nodo 2 
                                    {1, 4},         // Nodo 3
                                    {1, 2, 3} };    // Nodo 4
    MatrixXd matrWeights1(5, 5); 
    matrWeights1 << 0, 10, 1, 0, 0,
                   10, 0, 2, 9, 5,
                   1, 2, 0, 0, 20,
                   0, 9, 0, 0, 3,
                   0, 5, 20, 3, 0; 
    int v1_1 = 3;
    int v2_1 = 2;
    int numVert1 = 5;
    double lengthPath1 = 0.0;
    vector<int> minPathDijkstra1 = Dijkstra(adjList1, v1_1, v2_1, numVert1, matrWeights1, lengthPath1);
    vector<int> minPathEx1 = {3, 4, 1, 2};
    EXPECT_EQ(minPathDijkstra1, minPathEx1);
    EXPECT_DOUBLE_EQ(lengthPath1, 10.0);
}