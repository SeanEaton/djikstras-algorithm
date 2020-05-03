// ---------------------------------- graphm.h ----------------------------------------
// Sean Eaton CSS502
// Created: February 12th, 2020
// Last Modified: February 18th, 2020
// ------------------------------------------------------------------------------------
// Declaration for a 2d matrix graph, modeling distances between mutliple sources and
// destinations.
// ------------------------------------------------------------------------------------
// Utilizes nodedata class to store string data (description/place name) for each node 
// in the graph, with some comparison functionality.
// ------------------------------------------------------------------------------------

#include "nodedata.h"
#ifndef GRAPHM_H
#define GRAPHM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

const int MAXNODES = 100;

class GraphM {
    public:
        GraphM(); //sets table T's elements' visited values to false, distances to infinity, and paths to 0
        void buildGraph(ifstream&); //builds a graph, filling data with place names, populating table C (adjacency matrix), and setting size
        void insertEdge(int, int, int); //adds an edge with a given weight between two nodes
        void removeEdge(int, int); //removes an edge between two nodes
        void findShortestPath(); //populates table T with the shortest paths from every node to every other node
        void displayAll(); //displays the shortest paths from every node to every other node
        void display(int, int); //displays the shortest path from one node to another



    private:
        struct TableType {
            bool visited; //whether node is visited
            int dist; //distance to this node in current minimum distance path
            int path; //previous node in minimum distance path
        };

        NodeData data[MAXNODES]; //data for each node (in this case, place names)
        int C[MAXNODES][MAXNODES]; //adjacency matrix (distance between nodes connected by an edge)
        int size; //number of nodes in the graph
        TableType T[MAXNODES][MAXNODES]; //table storing paths of min distance

        //helper functions
        string pathByNode(int, int);
        void pathByName(int, int);
        void pathByNameHelper(int, int);
        void resetGraph();

};
#endif