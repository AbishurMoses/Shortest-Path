#include "Graph.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <queue>

// Code works. Display needs tuning

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
                cout << setw(13) << "-";
            }
            else {
                cout << setw(13)  << adjacency_matrix[i][j];
            }
        }
        cout << endl;
    }
}

Graph::Graph(const string& filename) {
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
    adjacency_matrix.resize(cities, vector<int>(cities, NO_CONNECTION));

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

        // Map unorder_map and vector with city label and index
        label_to_vertex_map[city] = city_i;
        auto it = vertex_to_label_map.begin() + city_i;
        vertex_to_label_map.insert(it, city);

        // Assigning city to i and weight to j
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
    // Can use static_cast as long as variable does not handle negative numbers
    return static_cast<int>(adjacency_matrix.size());
}

// Use constant int to access cities in unordered_map
// Access first half of triange. Access values till value is MAX_INT
int Graph::weight() const {
    int total_weight = 0;
    int i = 0;
    while (i < size()) {
        for (int j = 0; j < i; j++) {
            if (adjacency_matrix[i][j] != INT_MAX) {
                total_weight += adjacency_matrix[i][j];
            }
        }
        i++;
    }
    return total_weight;
}

int Graph::breadth_first_path_weight(const string& begin, const string& end) {
    queue<int> q;
    vector<int> visited(size(), 0);
    vector<int> distance(size(), NO_CONNECTION); // Distance array to store path weights
    int vector_index = 0;

    // If begin or end doesn't exist in unordered_map
    auto check_begin = label_to_vertex_map.find(begin);
    auto check_end = label_to_vertex_map.find(end);
    if ((check_begin == label_to_vertex_map.end()) || (check_end == label_to_vertex_map.end())) {
        return NO_CONNECTION;
    }

    // BFS algorithm
    // Finding index of begin and pushing to queue
    visited[label_to_vertex_map[begin]] = 1;
    distance[label_to_vertex_map[begin]] = 0; // Distance to the starting vertex is 0
    q.push(label_to_vertex_map[begin]);
    

    while (!q.empty()) {
        vector_index = q.front();
        q.pop();

        for (int i = 0; i < adjacency_matrix[vector_index].size(); i++) {
            if (visited[i] != 1 && adjacency_matrix[vector_index][i] != NO_CONNECTION) {
                visited[i] = 1;
                distance[i] = distance[vector_index] + adjacency_matrix[vector_index][i]; // Assuming unweighted graph, otherwise use the edge weight
                q.push(i);

                if (vertex_to_label_map[i] == end) {
                    // Found the destination vertex, return the distance
                    return distance[i];
                }
            }
        }
    }

    // If the loop completes and the destination was not reached, return NO_CONNECTION
    return NO_CONNECTION;
}

// Edge struct To store vertex1, vertex2, and weight
struct Edge {
    int vertex1;
    int vertex2;
    int weight;

    Edge(int v1, int v2, int w) : vertex1(v1), vertex2(v2), weight(w) {}
};

//Custom Comparator for Priority Queue
class Compare {
public:
    bool operator()(Edge edge1, Edge edge2) {
        return edge1.weight > edge2.weight;
    }
};

int Find(vector<int>& set, int i) {
    if (set[i] != i) {
        set[i] = Find(set, set[i]);
    }
    return set[i];
}

void Union(vector<int>& set, int s1, int s2) {
    int p1 = Find(set, set[s1]),
        p2 = Find(set, set[s2]);
    set[p1] = p2;
}

Graph Graph::kruskal() {
    priority_queue<Edge, vector<Edge>, Compare> pq;
    vector<int> set;
    vector<vector<int>> min_spanning_tree(size(), vector<int>(size(), NO_CONNECTION));

    // Making a set
    for (int i = 0; i < size(); ++i) {
        // Creating sets
        set.push_back(i);
        for (int j = 0; j < i; ++j) {
            // Pushing to priority queue to sort by weight increasing
            pq.push(Edge(i, j, adjacency_matrix[i][j]));
        }
    }

    // For every set, Union if Find(v1) doesn't match Find(v2)
    while (!pq.empty()) {
        Edge topEdge = pq.top();

        if (Find(set, topEdge.vertex1) != Find(set, topEdge.vertex2)) {
            Union(set, topEdge.vertex1, topEdge.vertex2);
            min_spanning_tree[topEdge.vertex1][topEdge.vertex2] = topEdge.weight;
            min_spanning_tree[topEdge.vertex2][topEdge.vertex1] = topEdge.weight;
        }
        pq.pop();
    }

    adjacency_matrix = min_spanning_tree;
    generate_graph(adjacency_matrix, vertex_to_label_map);
    return *this;
}

ostream& operator<<(ostream& os, const Graph& g) {
    // Placeholder value, replace with actual implementation
    os << "Graph details go here";
    return os;
}