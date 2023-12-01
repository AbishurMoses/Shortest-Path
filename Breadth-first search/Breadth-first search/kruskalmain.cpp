// KruskalsMST.cpp
// Created 11/5/2019 8:25:28 PM

#include <iostream>
#include "Graph.h"

int main() {
    Graph g{ "distance.txt" };
    std::cout << g << '\n';
    std::cout << "\n====================================================\n\n";
    Graph mst = g.kruskal();
    std::cout << mst << '\n';
    std::cout << "Graph edge weights: " << mst.weight() << '\n';
    int path_weight = mst.breadth_first_path_weight("London", "Moscow");
    if (path_weight == Graph::NO_CONNECTION) {
        std::cout << "No such path exists\n";
    }
    else {
        std::cout << "London to Moscow path length: " << path_weight << '\n';
    }

}