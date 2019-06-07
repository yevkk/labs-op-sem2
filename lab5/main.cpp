// tasks: 2, 3, 7, 13
// AS - Adjacency Structure;
// BV32 - Binary Vector with length = 32;

#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <algorithm>
#include <ctime>

const int MAX_WEIGHT = 15;
const int MAX_DATA = 50;
const int BITS_LONG = 32;
const long INF = LONG_MAX / 2 - 1;

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

    //-----
    void add_node(int data) {
        nodes.push_back(new GraphNodeAS(data));
    }

    void add_edge(int index1, int index2, int weight = 1) {
        if (!weighted) weight = 1;
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
            if (e.first != index)
                if (cycle_exist(e.first, false)) return true;
        }
        return false;
    }

    //-----
    void print_dfs(int index = 0, bool start = true) {
        static std::vector<bool> visited;
        if (start) {
            for (auto &e:nodes)
                visited.push_back(false);

            std::cout << "DFS:" << std::endl;

            for (int i = 0; i < nodes.size(); i++) {
                if (!visited[i]) {
                    print_dfs(i, false);
                    std::cout << std::endl;
                }
            }
        } else {
            std::cout << index + 1 << " ";
            visited[index] = true;

            for (auto &e:nodes[index]->adjacent_nodes)
                if (!visited[e.first]) print_dfs(e.first, false);
        }
    }

    static bool weights_sort_cmp(std::pair<int, int> pair1, std::pair<int, int> pair2) {
        return (pair1.second < pair2.second);
    }

    void print_dfs_by_weights(int index = 0, bool start = true) {
        if (!weighted) {
            print_dfs();
            return;
        }
        static std::vector<bool> visited;
        if (start) {
            for (auto &e:nodes)
                visited.push_back(false);

            std::cout << "DFS (by weights):" << std::endl;

            for (int i = 0; i < nodes.size(); i++) {
                if (!visited[i]) {
                    print_dfs_by_weights(i, false);
                    std::cout << std::endl;
                }
            }
        } else {
            std::cout << index + 1 << " ";
            visited[index] = true;

            auto vec = nodes[index]->adjacent_nodes;
            if (!vec.empty()) {
                std::sort(vec.begin(), vec.end(), weights_sort_cmp);

                for (auto &e:vec)
                    if (!visited[e.first]) print_dfs_by_weights(e.first, false);
            }
        }
    }

    //-----
    void floyds_algorithm(bool print = true) {
        std::vector<std::vector<int>> dist;
        std::vector<int> temp;

        for (auto &e:nodes) {
            temp.push_back(0);
        }

        for (auto &e:nodes) {
            dist.push_back(temp);
        }

        for (int i = 0; i < nodes.size(); i++) {
            for (auto &e:nodes[i]->adjacent_nodes) {
                dist[i][e.first] = e.second;
            }
        }

        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < nodes.size(); j++) {
                if ((i != j) && (dist[i][j] == 0)) dist[i][j] = INF;
                if (i == j) dist[i][j] = 0;
            }
        }

        for (int k = 0; k < nodes.size(); k++) {
            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < nodes.size(); j++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }

        if (print) {
            std::cout << "MIN DISTANCES:" << std::endl;

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

            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < 2 - std::to_string(i + 1).size(); j++)
                    std::cout << " ";
                std::cout << i + 1 << "|";

                for (int k = 0; k < nodes.size(); k++) {
                    if (dist[i][k] == INF) {
                        std::cout << "  -";
                    } else {
                        for (int j = 0; j < 3 - std::to_string(dist[i][k]).size(); j++)
                            std::cout << " ";
                        std::cout << dist[i][k];
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    //-----
    void tp_sort_visit(int index, std::vector<int> &marks, std::vector<int> &result) {
        if (marks[index] == 2) return;
        marks[index] = 1;
        for (int i = 0; i < nodes[index]->adjacent_nodes.size(); i++)
            if (nodes[index]->adjacent_nodes[i].first != index)
                tp_sort_visit(nodes[index]->adjacent_nodes[i].first, marks, result);
        marks[index] = 2;
        result.push_back(index);
    }

    void topological_sort(bool print = true) {
        if (print) std::cout << "TOPOLOGICAL SORT:" << std::endl;

        if (directed && !cycle_exist()) {
            std::vector<int> marks;
            std::vector<int> result;
            for (auto &e:nodes) marks.push_back(0);

            for (int i = 0; i < nodes.size(); i++)
                if (marks[i] == 0) tp_sort_visit(i, marks, result);

            if (print) {
                for (int k = result.size() - 1; k >= 0; k--)
                    std::cout << result[k] + 1 << " ";
                std::cout << std::endl;
            }

        } else {
            if (print) {
                std::cout << "Graph can't be sorted (is not directed or cycles exist);" << std::endl;
            }
        }
    }

    //----- not finished
    void spanning_tree_step(int index = 0, bool start = true) {
        static std::vector<bool> visited;
        if (start) {
            for (auto &e:nodes)
                visited.push_back(false);

            for (int i = 0; i < nodes.size(); i++) {
                if (!visited[i]) {
                    spanning_tree_step(i, false);
                }
            }
        } else {
            std::cout << index + 1 << " ";
            visited[index] = true;

            for (int i = 0; i < nodes[index]->adjacent_nodes.size(); i++)
                if ((nodes[index]->adjacent_nodes[i].first == index) ||
                    (visited[nodes[index]->adjacent_nodes[i].first])) {
                    nodes[index]->adjacent_nodes.erase(nodes[index]->adjacent_nodes.begin() + i);
                } else {
                    spanning_tree_step(nodes[index]->adjacent_nodes[i].first, false);
                }
        }
    }

    void spanning_tree(bool print = true) {
        GraphAS graph = *this;
        graph.spanning_tree_step();
        graph.print();
    }

    //-----
    void print() {
        std::cout << "Graph:" << std::endl;

        std::cout << "directed: " << std::boolalpha << directed << std::endl;

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
        nodes = {};
    }

    void add_node(int data) {
        nodes.push_back(new GraphNodeBV32(data));
    }

    void add_edge(unsigned int index1, unsigned int index2, int weight = 0) {
        if (!weighted) weight = 1;
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

        for (int i = 0; i < BITS_LONG; i++)
            if ((i != index) && (nodes[index]->is_adjacent(i)))
                if (cycle_exist(i, false)) return true;

        return false;
    }

    void print_dfs(int index = 0, bool start = true) {
        static bool visited[BITS_LONG];
        if (start) {
            for (auto &e:visited)
                e = false;

            std::cout << "DFS:" << std::endl;

            for (int i = 0; i < nodes.size(); i++) {
                if (!visited[i]) {
                    print_dfs(i, false);
                    std::cout << std::endl;
                }
            }
        } else {
            std::cout << index + 1 << " ";
            visited[index] = true;

            for (int i = 0; i < nodes.size(); i++)
                if (nodes[index]->is_adjacent(i))
                    if (!visited[i]) print_dfs(i, false);
        }
    }

    static bool weights_sort_cmp(std::pair<int, int> pair1, std::pair<int, int> pair2) {
        return (pair1.second < pair2.second);
    }

    void print_dfs_by_weights(int index = 0, bool start = true) {
        if (!weighted) {
            print_dfs();
            return;
        }
        static bool visited[BITS_LONG];
        if (start) {
            for (auto &e:visited)
                e = false;

            std::cout << "DFS (by weights):" << std::endl;

            for (int i = 0; i < nodes.size(); i++) {
                if (!visited[i]) {
                    print_dfs_by_weights(i, false);
                    std::cout << std::endl;
                }
            }
        } else {
            std::cout << index + 1 << " ";
            visited[index] = true;

            std::vector<std::pair<int, int>> vec;
            for (int i = 0; i < nodes.size(); i++)
                if (nodes[index]->is_adjacent(i))
                    vec.emplace_back(i, nodes[index]->weights[i]);

            if (!vec.empty()) {
                std::sort(vec.begin(), vec.end(), weights_sort_cmp);

                for (auto &e:vec)
                    if (!visited[e.first]) print_dfs_by_weights(e.first, false);
            }
        }
    }

    void floyds_algorithm(bool print = true) {
        int dist[BITS_LONG][BITS_LONG];

        for (int i = 0; i < nodes.size(); i++) {
            for (int j = 0; j < nodes.size(); j++) {
                if (i == j) dist[i][j] = 0;
                else if (nodes[i]->weights[j] == 0) dist[i][j] = INF;
                else dist[i][j] = nodes[i]->weights[j];
            }
        }

        for (int k = 0; k < nodes.size(); k++) {
            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < nodes.size(); j++) {
                    if (dist[i][k] + dist[k][j] < dist[i][j])
                        dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }

        if (print) {
            std::cout << "Min distances:" << std::endl;

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

            for (int i = 0; i < nodes.size(); i++) {
                for (int j = 0; j < 2 - std::to_string(i + 1).size(); j++)
                    std::cout << " ";
                std::cout << i + 1 << "|";

                for (int k = 0; k < nodes.size(); k++) {
                    if (dist[i][k] == INF) {
                        std::cout << "  -";
                    } else {
                        for (int j = 0; j < 3 - std::to_string(dist[i][k]).size(); j++)
                            std::cout << " ";
                        std::cout << dist[i][k];
                    }
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void tp_sort_visit(int index, int *marks, std::vector<int> &result) {
        if (marks[index] == 2) return;
        marks[index] = 1;
        for (int i = 0; i < BITS_LONG; i++)
            if ((nodes[index]->is_adjacent(i)) && (i != index))
                tp_sort_visit(i, marks, result);
        marks[index] = 2;
        result.push_back(index);
    }

    void topological_sort(bool print = true) {
        if (print) std::cout << "Topological sort:" << std::endl;

        if (directed && !cycle_exist()) {
            int marks[BITS_LONG];
            std::vector<int> result;
            for (auto &e:marks) e = 0;

            for (int i = 0; i < nodes.size(); i++)
                if (marks[i] == 0) tp_sort_visit(i, marks, result);

            if (print) {
                for (int k = result.size() - 1; k >= 0; k--)
                    std::cout << result[k] + 1 << " ";
                std::cout << std::endl;
            }

        } else {
            if (print) {
                std::cout << "Graph can't be sorted (is not directed or cycles exist);" << std::endl;
            }
        }
    }

    void print() {
        std::cout << "Graph:" << std::endl;

        std::cout << "directed: " << std::boolalpha << directed << std::endl;

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

//--------------- DEMO MODE ---------------
void demo_graph_as() {
    std::cout << "=============== ADJACENCY STRUCTURE DEMO ===============" << std::endl << std::endl;
    int nodes = -2, edges = -2;

    while ((nodes < -1) || (nodes > 30)) {
        std::cout << "Input nodes number (0 - 30, -1 to set random):";
        std::cin >> nodes;
    }
    if (nodes == -1) {
        nodes = rand_int(5, 20);
        std::cout << "nodes number = " << nodes << std::endl;
    }

    while ((edges < -1) || (edges > nodes * nodes)) {
        std::cout << "Input edges number (0 - nodes^2, -1 to set random):";
        std::cin >> edges;
    }
    if (edges == -1) edges = rand_int(5, nodes * nodes);

    GraphAS graph = random_graph_as(nodes, edges);
    std::cout << "Created random graph with " << nodes << "nodes and " << edges << "edges;" << std::endl << std::endl;
    graph.print();

    std::cout << "CYCLE EXISTS:" << std::boolalpha << graph.cycle_exist() << std::endl << std::endl;

    graph.print_dfs();
    std::cout << std::endl;

    if (graph.weighted) {
        graph.print_dfs_by_weights();
        std::cout << std::endl;
    }

    graph.floyds_algorithm();
    std::cout << std::endl;

    graph.topological_sort();
    std::cout << std::endl;

    GraphBV32 graph2 = transform_graph(graph);
    std::cout << "Graph transformed to binary vector 32" << std::endl;
    graph2.print();

    std::cout << "=========================================" << std::endl << std::endl;
}

void demo_graph_bv32() {
    std::cout << "=============== BINARY VECTOR 32 DEMO ===============" << std::endl << std::endl;
    int nodes = -2, edges = -2;

    while ((nodes < -1) || (nodes > 32)) {
        std::cout << "Input nodes number (0 - 32, -1 to set random):";
        std::cin >> nodes;
    }
    if (nodes == -1) {
        nodes = rand_int(5, 20);
        std::cout << "nodes number = " << nodes << std::endl;
    }

    while ((edges < -1) || (edges > nodes * nodes)) {
        std::cout << "Input edges number (0 - nodes^2, -1 to set random):";
        std::cin >> edges;
    }
    if (edges == -1) edges = rand_int(5, nodes * nodes);

    GraphBV32 graph = random_graph_bv32(nodes, edges);
    std::cout << "Created random graph with " << nodes << "nodes and " << edges << "edges;" << std::endl << std::endl;
    graph.print();

    std::cout << "CYCLE EXISTS:" << std::boolalpha << graph.cycle_exist() << std::endl << std::endl;

    graph.print_dfs();
    std::cout << std::endl;

    if (graph.weighted) {
        graph.print_dfs_by_weights();
        std::cout << std::endl;
    }

    graph.floyds_algorithm();
    std::cout << std::endl;

    graph.topological_sort();
    std::cout << std::endl;

    GraphAS graph2 = transform_graph(graph);
    std::cout << "Graph transformed to adjacency structure" << std::endl;
    graph2.print();

    std::cout << "=========================================" << std::endl << std::endl;
}

//--------------- BENCHMARK MODE ---------------

//--------------- INTERACTIVE MODE ---------------
void interactive_graph_as() {
    GraphAS graph = random_graph_as(0, 0);
    int nodes, edges, command, val1, val2, val3;
    while (true) {
        command = -1;
        std::cout
                << "\nEnter command:\n"
                   "\t#0 - print\n"
                   "\t#1 - create new graph;\n"
                   "\t#2 - add node;\n"
                   "\t#3 - add edge\n"
                   "\t#4 - dfs\n"
                   "\t#5 - dfs by weights\n"
                   "\t#6 - min distances\n"
                   "\t#7 - topological sort\n"
                   "\t#8 - demo\n"
                   "#9 - back\n";
        while ((command < 0) || (command > 9)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 0: {
                graph.print();
                break;
            }
            case 1: {
                graph = random_graph_as(0, 0);
                val1 = 2;
                while ((val1 > 1) || (val1 < 0)) {
                    std::cout << "Is weighted (0 or 1):";
                    std::cin >> val1;
                }
                graph.weighted = val1;
                val1 = 2;
                while ((val1 > 1) || (val1 < 0)) {
                    std::cout << "Is directed (0 or 1):";
                    std::cin >> val1;
                }
                graph.directed = val1;
                break;
            }
            case 2: {
                std::cout << "data = ";
                std::cin >> val1;
                graph.add_node(val1);
                break;
            }
            case 3: {
                val1 = val2 = val3 = -1;
                while ((val1 > graph.nodes.size()) || (val1 < 1)) {
                    std::cout << "First node:";
                    std::cin >> val1;
                }
                while ((val2 > graph.nodes.size()) || (val2 < 1)) {
                    std::cout << "Second node:";
                    std::cin >> val2;
                }
                if (graph.weighted) {
                    while (val3 < 1) {
                        std::cout << "Weight:";
                        std::cin >> val3;
                    }
                } else {
                    val3 = 1;
                }
                graph.add_edge(val1 - 1, val2 - 1, val3);
                break;
            }
            case 4: {
                graph.print_dfs();
                break;
            }
            case 5: {
                graph.print_dfs_by_weights();
                break;
            }
            case 6: {
                graph.floyds_algorithm();
                break;
            }
            case 7: {
                graph.topological_sort();
                break;
            }
            case 8: {
                demo_graph_as();
                break;
            }
            case 9: {
                return;
            }
            default:;
        }
    }
}

void interactive_graph_bv32() {
    GraphBV32 graph = random_graph_bv32(0, 0);
    int nodes, edges, command, val1, val2, val3;
    while (true) {
        command = -1;
        std::cout
                << "\nEnter command:\n"
                   "\t#0 - print\n"
                   "\t#1 - create new graph;\n"
                   "\t#2 - add node;\n"
                   "\t#3 - add edge\n"
                   "\t#4 - dfs\n"
                   "\t#5 - dfs by weights\n"
                   "\t#6 - min distances\n"
                   "\t#7 - topological sort\n"
                   "\t#8 - demo\n"
                   "#9 - back\n";
        while ((command < 0) || (command > 9)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 0: {
                graph.print();
                break;
            }
            case 1: {
                graph = random_graph_bv32(0, 0);
                val1 = 2;
                while ((val1 > 1) || (val1 < 0)) {
                    std::cout << "Is weighted (0 or 1):";
                    std::cin >> val1;
                }
                graph.weighted = val1;
                val1 = 2;
                while ((val1 > 1) || (val1 < 0)) {
                    std::cout << "Is directed (0 or 1):";
                    std::cin >> val1;
                }
                graph.directed = val1;
                break;
            }
            case 2: {
                std::cout << "data = ";
                std::cin >> val1;
                graph.add_node(val1);
                break;
            }
            case 3: {
                val1 = val2 = val3 = -1;
                while ((val1 > graph.nodes.size()) || (val1 < 1)) {
                    std::cout << "First node:";
                    std::cin >> val1;
                }
                while ((val2 > graph.nodes.size()) || (val2 < 1)) {
                    std::cout << "Second node:";
                    std::cin >> val2;
                }
                if (graph.weighted) {
                    while (val3 < 1) {
                        std::cout << "Weight:";
                        std::cin >> val3;
                    }
                } else {
                    val3 = 1;
                }
                graph.add_edge(val1 - 1, val2 - 1, val3);
                break;
            }
            case 4: {
                graph.print_dfs();
                break;
            }
            case 5: {
                graph.print_dfs_by_weights();
                break;
            }
            case 6: {
                graph.floyds_algorithm();
                break;
            }
            case 7: {
                graph.topological_sort();
                break;
            }
            case 8: {
                demo_graph_bv32();
                break;
            }
            case 9: {
                return;
            }
            default:;
        }
    }
}

int main() {
    interactive_graph_as();
    demo_graph_as();
    demo_graph_bv32();

    return 0;
}

