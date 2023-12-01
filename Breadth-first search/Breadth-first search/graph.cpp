#include "Graph.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <queue>
#include <numeric>

// Everything works fine.
// Need to work on breadth_first_path_weight and kruskal's algo
// Check all #includes and see if any are unnecessary

// Once you understand size_t to int remove all static_cast

using namespace std;

// Draw Graph
void generate_graph(vector<vector<int>>& adjacency_matrix, vector<string>& vertex_to_label_map) {
    size_t vertex_size = adjacency_matrix.size();
    int num_of_cities = 0;
    // Print Headers
    cout << setw(12) << " ";
    for (int i = 0; i < vertex_size; i++) {
        cout << right << setw(6) << " " << vertex_to_label_map[i];
        num_of_cities++;
    }
    cout << endl;

    // Print seperator
    cout << setw(12) << right << " " << "+" << setfill('-') << setw(num_of_cities * 13) << "" << std::setfill(' ') << std::endl;

    //Print contents
    for (int i = 0; i < vertex_size; i++) {
        cout << right << setw(12)  << vertex_to_label_map[i] << "|";
        for (int j = 0; j < vertex_size; j++) {
            if (adjacency_matrix[i][j] == INT_MAX) {
                cout << right << setw(13) << "-";
            }
            else {
                cout << right << setw(13)  << adjacency_matrix[i][j];
            }
        }
        cout << endl;
    }
}

Graph::Graph(const string& filename) {
    //Will contain key value pairs of cities and 
    unordered_map<string, int> umap;

    // Opening File
    ifstream file(filename);

    // Error checking if file failed to open
    if (!file.is_open()) {
        cout << "Error Opening File " << filename;
        return;
    }

    // Initializing matrix size based on number of lines (one city per line)
    int cities = 0;
    string allline;
    while (getline(file, allline)) {
        if (!allline.empty()) {
            cities++;
        }
    }
    // Decrementing cities because of empty line at top of file
cities--;

// Resetting file stream
file.clear();
file.seekg(0, ios::beg);

// Setting outer loop to cities, inner loop num of elements with "empty values", Graph::NO_CONNECTION
adjacency_matrix.resize(cities, vector<int>(cities, Graph::NO_CONNECTION));

// Read and process the header line
string header;
getline(file, header);

string line;
int i = 0;
int city_i = 0;
while (getline(file, line)) {
    istringstream iss(line);
    string city;
    iss >> city; // Read the city name

    label_to_vertex_map[city] = city_i;
    auto it = vertex_to_label_map.begin() + city_i;
    vertex_to_label_map.insert(it, city);

    umap[city] = i; //Map to unordered_list

    // Assigning city index to i and value index is j
    int value;
    int value_i = 0;
    while (iss >> value || !iss.eof()) {
        if (iss.fail()) {
            adjacency_matrix[city_i][value_i] = Graph::NO_CONNECTION;
            iss.clear();
        }
        else {
            adjacency_matrix[city_i][value_i] = value;
        }
        value_i++;
    }
    city_i++;
}

generate_graph(adjacency_matrix, vertex_to_label_map);
file.close();
}

int Graph::size() const {
    // Static_cast suppress error ('return': conversion from 'size_t' to 'int', possible loss of data
    return static_cast<int>(adjacency_matrix.size());
}

// Use constant int to access cities in unordered_map
// Access till value is MAX_INT
// Go to next city. Since value after MAX_INT will be repeating
int Graph::weight() const {
    int total_weight = 0;
    int i = 0;
    // Sum half of edge values and double the result
    while (i < size()) {
        ;
        for (int j = 0; j < i; j++) {
            if (adjacency_matrix[i][j] != INT_MAX) {
                total_weight += adjacency_matrix[i][j];
            }
        }
        i++;
    }
    return total_weight * 2;
}

int Graph::breadth_first_path_weight(const string& begin, const string& end) {
    // If begin or end doesn't exist in unordered_map
    auto check_begin = label_to_vertex_map.find(begin);
    auto check_end = label_to_vertex_map.find(end);
    if ((check_begin == label_to_vertex_map.end()) || (check_end == label_to_vertex_map.end())) {
        return NO_CONNECTION;
    }

    return 0;
}

// To store vertex1, vertex2, and weight
struct Edge {
    int vertex1;
    int vertex2;
    int weight;

    Edge(int v1, int v2, int w) : vertex1(v1), vertex2(v2), weight(w) {}
};

Graph Graph::kruskal() {
    priority_queue<Edge, vector<Edge>, function<bool(const Edge&, const Edge&) >> pq;

    // Populate edges vector from the adjacency matrix
    // Making a set
    for (int i = 0; i < size(); ++i) {
        for (int j = 0; j < i; ++j) {
            pq.push(Edge(i, j, adjacency_matrix[i][j]));
        }
    }

    while (!pq.empty()) {
        cout << pq.top() << endl;
        pq.pop();
    }

    return *this;
}

ostream& operator<<(ostream& os, const Graph& g) {
    // Placeholder value, replace with actual implementation
    os << "Graph details go here";
    return os;
}