// --------------------------------- graphm.cpp ---------------------------------------
// Sean Eaton CSS502
// Created: February 12th, 2020
// Last Modified: February 18th, 2020
// ------------------------------------------------------------------------------------
// Implementaion for a 2d matrix graph, modeling distances between mutliple sources and
// destinations.
// ------------------------------------------------------------------------------------
// Utilizes nodedata class to store string data (description/place name) for each node 
// in the graph, with some comparison functionality.
// ------------------------------------------------------------------------------------

#include "graphm.h"

/*
*------------------------------ CONSTRUCTORS/DESTRUCTORS ------------------------------
*/
//-------------------------------- constructor ----------------------------------------
// Default contructor, initializing size, populating C and T tables to default values.
// Preconditions: None.
// Postconditions: A complete Graph object, with empty adjacency matrix and path finder 
// table.
GraphM::GraphM() {
    size = 0;
    for (int i = 1; i < MAXNODES; i++) { //setting table C (adjacency matrix) variables to defaults
        for (int j = 1; j < MAXNODES; j++) {
            C[i][j] = numeric_limits<int>::max();
        }
    }
    for (int i = 1; i < MAXNODES; i++) { //setting table T (shortest path finder) variables to defaults
        for (int j = 1; j < MAXNODES; j++) {
            T[i][j].visited = false;
            T[i][j].dist = numeric_limits<int>::max();
            T[i][j].path = 0; //no node has the number 0, so this indicates an unconstructed path
        }
    }
}

/*
*------------------------------------- MUTATORS ---------------------------------------
*/
//--------------------------------- buildGraph ----------------------------------------
// Sets this Graph Object's size, sets place names, and populates it's adjacency matrix 
// with values. Will also clear the Graph's data should client build over an existing 
// graph.
// Preconditions: A constructed Graph.
// Postconditions: A populated adjacency matrix and data array.
void GraphM::buildGraph(ifstream& infile) {
    if (size != 0) resetGraph();
    string line;
    int line_count = 0;
    while (getline(infile, line)) {
        if (line_count == 0) { //get size
            size = stoi(line);
        }
        else if (line_count != 0 && line_count <= size) { //populate data with node place names
            NodeData new_data = NodeData(line);
            data[line_count] = new_data;
        }
        else { //populate table C (adjacency matrix) with edge weights
            istringstream iss(line);
            string from;
            string to;
            string weight;
            iss >> from;
            if (from[0] == '0') break;
            iss >> to;
            iss >> weight;
            insertEdge(stoi(from), stoi(to), stoi(weight));
        }
        line_count++;
    }
}

//--------------------------------- insertEdge ----------------------------------------
// Inserts a single edge between two vertices. 
// Preconditions: A constructed Graph.
// Postconditions: An element of the adjacency matrix will be populated with new value.
void GraphM::insertEdge(int from, int to, int weight) {
    if ((from > 0 && from <= size) && (to > 0 && to <= size) && weight >= 0) C[from][to] = weight;
}

//--------------------------------- removeEdge ----------------------------------------
// Removes an edge between two vertices. 
// Preconditions: A constructed Graph.
// Postconditions: An element of the adjacency matrix will be populated with new value.
void GraphM::removeEdge(int from, int to) {
    if ((from > 0 && from < MAXNODES) && (to > 0 && to < MAXNODES)) C[from][to] = numeric_limits<int>::max();
}

//------------------------------- findShortestPath ------------------------------------
// Populates shortest path table (T) with the shortest paths from every source to every
// possible destination.
// Preconditions: A constructed Graph.
// Postconditions: The shortest path table (T) will contain all data pertaining to the
// shortest paths between each node and every other node.
void GraphM::findShortestPath() {
    for (int source = 1; source <= size; source++) {
        T[source][source].dist = 0; //sets the origin to 0
        for (int i = 1; i <= size; i++) {
            int from = 0;
            int min_path = numeric_limits<int>::max();
            bool continuing = false;
            for (int j = 1; j <= size; j++) {
                if (T[source][j].dist < min_path && T[source][j].visited == false) { //find next shortest unvisited distance
                    from = j;
                    min_path = T[source][j].dist;
                    continuing = true;
                }
            } 
            if (continuing == true) {
                T[source][from].visited = true;
                for (int to = 1; to <= size; to++) {
                    if (C[from][to] != numeric_limits<int>::max() && T[source][to].visited == false) {
                        if (T[source][to].dist > (T[source][from].dist + C[from][to])) T[source][to].path = from;
                        T[source][to].dist = min(T[source][to].dist, (T[source][from].dist + C[from][to]));
                    }
                }
            }
        }
    }
}

//---------------------------------- resetGraph ---------------------------------------
// Clears this Graph object's data members to default values.
// Preconditions: A constructed Graph.
// Postconditions: A graph with the same data as a newly constructed Graph.
void GraphM::resetGraph() {
    size = 0;
    for (int i = 1; i < MAXNODES; i++) { //setting table C (adjacency matrix) variables to defaults
        for (int j = 1; j < MAXNODES; j++) {
            C[i][j] = numeric_limits<int>::max();
        }
    }
    for (int i = 1; i < MAXNODES; i++) { //setting table T (shortest path finder) variables to defaults
        for (int j = 1; j < MAXNODES; j++) {
            T[i][j].visited = false;
            T[i][j].dist = numeric_limits<int>::max();
            T[i][j].path = 0; //no node has the number 0, so this indicates an unconstructed path
        }
    }
}

/*
*------------------------------------ ACCESSORS ---------------------------------------
*/
//---------------------------------- displayAll ---------------------------------------
// Prints out all data pertaining to shortest paths between every combination of two 
// vertices.
// Preconditions: A constructed Graph.
// Postconditions: A complete output detailing shortest path data.
void GraphM::displayAll() {
    cout << "Description                From node  To node   Dijkstra's      Path" << endl;
    for (int from = 1; from <= size; from++) {
        cout << data[from] << endl;
        for (int to = 1; to <= size; to++) {
            if (from != to) {
                cout << setw(36) << from << setw(9) << to << setw(13) << (T[from][to].dist < numeric_limits<int>::max() ? to_string(T[from][to].dist) : "---") << "      " << pathByNode(from, to) << endl;
            }
        }
    }
}

//------------------------------------ display ----------------------------------------
// Prints out the data pertaining to the path between two vertices.
// Preconditions: A constructed Graph.
// Postconditions: A complete output detailing the distance between two vertices as 
// well as the path in terms of place number and place name.
void GraphM::display(int from, int to) {
    if ((from > 0 && from <= size) && (to > 0 && to <= size)) {
        cout << from << setw(9) << to << setw(13) << (T[from][to].dist < numeric_limits<int>::max() ? to_string(T[from][to].dist) : "no path") << "      " << pathByNode(from, to) << endl;
        pathByName(from, to);
    }
}

//---------------------------------- pathByNode ---------------------------------------
// Constructs a string representation of the path between two vertices in place 
// numbers.
// Preconditions: A constructed Graph.
// Postconditions: A string representation of the path between two vertices in place 
// numbers.
string GraphM::pathByNode(int from, int to) {
    if (T[from][to].path != 0) {
        string path_string;
        int current = to;
        while (current != 0) {
            path_string = to_string(current) + " " + path_string;
            current = T[from][current].path;
        }
        return path_string;
    }
    return "";
}

//---------------------------------- pathByName ---------------------------------------
// Constructs a string representation of the path between two vertices in place names.
// Preconditions: A constructed Graph.
// Postconditions: A string representation of the path between two vertices in place 
// names.
void GraphM::pathByName(int from, int to) {
    if (T[from][to].path != 0) {
        int current = to;
        pathByNameHelper(from, current);
    }
}

//------------------------------- pathByNameHelper ------------------------------------
// Helper function for pathByName. Constructs a string representation of the path 
// between two vertices in place names.
// Preconditions: A constructed Graph.
// Postconditions: A string representation of the path between two vertices in place 
// names.
void GraphM::pathByNameHelper(int from, int current) {
    if (current != 0) {
        pathByNameHelper(from, T[from][current].path);
        cout << data[current] << endl;
    }
}