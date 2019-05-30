// tasks: 2, 3, 7, 13
// AS - Adjacency Structure;
// BV32 - Binary Vector with length = 32;

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

const int MAX_WEIGHT = 99;
const int MAX_DATA = 50;
const int BITS_LONG = 32;

//--------------- RANDOM ---------------

double rand_double(double min, double max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_real_distribution<double> dist(min, max);

    return dist(gen);
}

int rand_int(int min, int max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

//--------------- TYPES ---------------
//AS
struct GraphNodeAS {
    int data;
    std::vector<std::pair<int, int>> adjacent_nodes; // pair: first - adjacent node index, second - edge weight;

    explicit GraphNodeAS(int Data) {
        data = Data;
        adjacent_nodes = {};
    }

    bool is_adjacent(int index) {
        for (auto &e:adjacent_nodes) {
            if (e.first == index) return true;
        }
        return false;
    }
};

struct GraphAS {
    std::vector<GraphNodeAS *> nodes;
    bool weighted;
    bool directed;

    GraphAS(bool Weighted, bool Directed) {
        weighted = Weighted;
        directed = Directed;
    }

    void add_node(int data) {
        nodes.push_back(new GraphNodeAS(data));
    }

    void add_edge(int index1, int index2, int weight = 0) {
        if ((index1 < nodes.size()) && (index2 < nodes.size())) {
            if (!nodes[index1]->is_adjacent(index2)) {
                if (directed) {
                    nodes[index1]->adjacent_nodes.emplace_back(index2, weight);
                } else {
                    nodes[index1]->adjacent_nodes.emplace_back(index2, weight);
                    nodes[index2]->adjacent_nodes.emplace_back(index1, weight);
                }
            }
        }
    }

    bool cycle_exist(int index = 0, bool start = true) {
        static std::vector<bool> visited;
        if (start) {
            visited.clear();
            for (auto &n:nodes) visited.emplace_back(false);
        }

        if (visited[index]) return true;
        visited[index] = true;
        for (auto &e:nodes[index]->adjacent_nodes) {
            if (((e.first != index) && (directed)) || ((!directed) && (e.first > index)))
                if (cycle_exist(e.first, false)) return true;
        }
        return false;
    }

    void print() {
        std::cout << "Graph:" << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i + 1).size(); j++)
                std::cout << " ";
            std::cout << i + 1;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        int num = 0;
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 2 - std::to_string(i + 1).size(); j++)
                std::cout << " ";
            std::cout << i + 1 << "|";

            for (int k = 0; k < nodes.size(); k++) {
                num = 0;
                if (weighted) {
                    for (auto &e:nodes[i]->adjacent_nodes) {
                        if (e.first == k) {
                            num = e.second;
                        }
                    }
                } else num = nodes[i]->is_adjacent(k);
                for (int j = 0; j < 3 - std::to_string(num).size(); j++)
                    std::cout << " ";
                std::cout << num;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;


    }

};

//BV32
struct GraphNodeBV32 {
    int data;
    unsigned long adjacency_vector;
    int weights[BITS_LONG]{};

    explicit GraphNodeBV32(int Data) {
        data = Data;
        adjacency_vector = 0;
        for (auto &e:weights)
            e = 0;
    }

    bool is_adjacent(unsigned int index) {
        return ((adjacency_vector >> index) & 01u);
    }
};

struct GraphBV32 {
    std::vector<GraphNodeBV32 *> nodes;
    bool weighted;
    bool directed;

    GraphBV32(bool Weighted, bool Directed) {
        weighted = Weighted;
        directed = Directed;
    }

    void add_node(int data) {
        nodes.push_back(new GraphNodeBV32(data));
    }

    void add_edge(unsigned int index1, unsigned int index2, int weight = 0) {
        if ((index1 < nodes.size()) && (index2 < nodes.size())) {
            if (!nodes[index1]->is_adjacent(index2)) {
                if (directed) {
                    nodes[index1]->adjacency_vector = nodes[index1]->adjacency_vector | (01u << (index2));
                    nodes[index1]->weights[index2] = weight;
                } else {
                    nodes[index1]->adjacency_vector = nodes[index1]->adjacency_vector | (01u << (index2));
                    nodes[index2]->adjacency_vector = nodes[index2]->adjacency_vector | (01u << (index1));
                    nodes[index1]->weights[index2] = weight;
                    nodes[index2]->weights[index1] = weight;
                }
            }
        }
    }

    bool cycle_exist(int index = 0, bool start = true) {
        static bool visited[BITS_LONG];
        if (start) {
            for (auto &e:visited) e = false;
        }

        if (visited[index]) return true;
        visited[index] = true;
        if (directed) {
            for (int i = 0; i < BITS_LONG; i++)
                if ((i != index) && (nodes[index]->is_adjacent(i)))
                    if (cycle_exist(i, false)) return true;
        } else {
            for (int i = index + 1; i < BITS_LONG; i++)
                if (nodes[index]->is_adjacent(i))
                    if (cycle_exist(i, false)) return true;
        }
        return false;
    }

    void print() {
        std::cout << "Graph:" << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 3 - std::to_string(i + 1).size(); j++)
                std::cout << " ";
            std::cout << i + 1;
        }
        std::cout << std::endl;

        std::cout << "   ";
        for (int i = 0; i < nodes.size(); i++) {
            std::cout << "---";
        }
        std::cout << std::endl;

        int num = 0;
        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < 2 - std::to_string(i + 1).size(); j++)
                std::cout << " ";
            std::cout << i + 1 << "|";

            for (int k = 0; k < nodes.size(); k++) {
                num = 0;
                if (weighted) {
                    if (nodes[i]->is_adjacent(k)) {
                        num = nodes[i]->weights[k];
                    }
                } else num = nodes[i]->is_adjacent(k);
                for (int j = 0; j < 3 - std::to_string(num).size(); j++)
                    std::cout << " ";
                std::cout << num;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;


    }
};

//--------------- RANDOM GRAPHS ---------------
GraphAS random_graph_as(int nodes_num, int edges_num) {
    GraphAS graph(rand_int(0, 1), rand_int(0, 1));

    for (int i = 0; i < nodes_num; i++)
        graph.add_node(rand_int(1, MAX_DATA));

    for (int i = 0; i < edges_num; i++)
        graph.add_edge(rand_int(0, graph.nodes.size() - 1), rand_int(0, graph.nodes.size() - 1),
                       rand_int(1, MAX_WEIGHT));

    return graph;
}

GraphBV32 random_graph_bv32(int nodes_num, int edges_num) {
    GraphBV32 graph(rand_int(0, 1), rand_int(0, 1));

    for (int i = 0; i < nodes_num; i++)
        graph.add_node(rand_int(1, MAX_DATA));

    for (int i = 0; i < edges_num; i++)
        graph.add_edge(rand_int(0, graph.nodes.size() - 1), rand_int(0, graph.nodes.size() - 1),
                       rand_int(1, MAX_WEIGHT));

    return graph;
}

//--------------- TRANSFORM GRAPH ---------------
GraphAS transform_graph(GraphBV32 &graph) {
    GraphAS result(graph.weighted, graph.directed);

    for (int i = 0; i < graph.nodes.size(); i++) {
        result.add_node(graph.nodes[i]->data);
        for (int j = 0; j < graph.nodes.size(); j++) {
            if (graph.nodes[i]->is_adjacent(j)) {
                result.add_edge(i, j, graph.nodes[i]->weights[j]);
            }
        }
    }
    return result;
}

GraphBV32 transform_graph(GraphAS &graph) {
    GraphBV32 result(graph.weighted, graph.directed);

    for (int i = 0; i < graph.nodes.size(); i++) {
        result.add_node(graph.nodes[i]->data);
        for (auto &p:graph.nodes[i]->adjacent_nodes) {
            result.add_edge(i, p.first, p.second);
        }
    }
    return result;
}

int main() {
    GraphAS graph1 = random_graph_as(7, 15);
    graph1.print();
    std::cout << "Cycle: " << graph1.cycle_exist() << " " << graph1.cycle_exist() << std::endl;
    GraphBV32 graph2 = transform_graph(graph1);
    graph2.print();
    std::cout << "Cycle: " << graph2.cycle_exist() << " " << graph2.cycle_exist() << std::endl;
    graph1 = random_graph_as(7, 10);
    //graph1.print();
    graph1 = transform_graph(graph2);
    graph1.print();
    std::cout << "Cycle: " << graph1.cycle_exist() << " " << graph1.cycle_exist() << std::endl;

    std::cout << std::endl << std::endl;



    return 0;
}

