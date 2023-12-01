#include <vector>
#include <unordered_map>

#include <string>

/**
 * Represents undirected graphs with string vertex labels
 * and integer edge weights.
 */
class Graph {
    /** The adjacency matrix data structure holding edge infomation. */
    std::vector<std::vector<int>> adjacency_matrix;

    /** Allows a fast lookup of a vertex number given its label */
    std::unordered_map<std::string, int> label_to_vertex_map;
    /** Allows a fast lookup of a vertex label given its number */
    std::vector<std::string> vertex_to_label_map;

public:
    /** An edge value that represents no connection. */
    static const int NO_CONNECTION = INT_MAX;

    /**
     * Creates a new graph object based on adjacency infomation stored in a text file.
     * filename: the file storing adjacency information.
     */
    Graph(const std::string& filename);

    /**
     * Returns the number of vertices in the graph
     */
    int size() const;

    /**
     * Returns the sum of the weight of all the edges in the graph.
     */
    int weight() const;

    /**
     * Computes the weight of the path connecting vertex begin to vertex end.
     * Returns NO_CONNECTION if no path exists between begin and end or if
     * the list does not contain both vertex labels.
     */
    int breadth_first_path_weight(const std::string& begin, const std::string& end);

    /**
     * Builds and returns a minimum spanning tree of this graph.
     */
    Graph kruskal(); 

    /**
     * Global overloaded << function that allows clients to print a graph object in a
     * convenient manner.  The Graph class befriends this function so that this function
     * can get to the private details of Graph objects to do its work.
     */
    friend std::ostream& operator<<(std::ostream& os, const Graph& g);
};


