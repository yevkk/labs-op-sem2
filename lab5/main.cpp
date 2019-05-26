// tasks: 2, 3, 7, 13
// AS - Adjacency Structure;
// BV32 - Binary Vector with length = 32;

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

const int MAX_WEIGHT = 99;
const int MAX_DATA = 50;

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
        if ((index1 <= nodes.size()) && (index2 <= nodes.size())) {
            if (!nodes[index1 - 1]->is_adjacent(index2 - 1)) {
                if (directed) {
                    nodes[index1 - 1]->adjacent_nodes.emplace_back(index2 - 1, weight);
                } else {
                    nodes[index1 - 1]->adjacent_nodes.emplace_back(index2 - 1, weight);
                    nodes[index2 - 1]->adjacent_nodes.emplace_back(index1 - 1, weight);
                }
            }
        }
    }

    void print_graph() {
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

struct GraphNodeBV32 {
    int data;
    int adjacency_vector;
    std::vector<int> weights;

    explicit GraphNodeBV32(int Data) {
        data = Data;
        adjacency_vector = 0;
    }
};

struct GraphBV32 : std::vector<GraphNodeBV32 *> {
    bool weighted;


};

//--------------- RANDOM GRAPHS ---------------
GraphAS random_graph(int nodes_num, int edges_num) {
    GraphAS graph(rand_int(0, 1), rand_int(0, 1));

    for (int i = 0; i < nodes_num; i++)
        graph.add_node(rand_int(1, MAX_DATA));

    for (int i = 0; i < edges_num; i++)
        graph.add_edge(rand_int(1, graph.nodes.size() - 1), rand_int(1, graph.nodes.size() - 1),
                       rand_int(1, MAX_WEIGHT));

    return graph;
}

int main() {
    GraphAS graph = random_graph(7, 10);
    graph.print_graph();
    return 0;
}