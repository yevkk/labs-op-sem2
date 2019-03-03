#include <iostream>
#include <vector>
#include <random>
#include <ctime>

double rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_real_distribution<double> dist(0, max);

    return dist(gen);
}

const double chance = 5;

struct Node {
    int data = (int) 0;
    Node *next = nullptr;
    Node *bottom = nullptr;
};
typedef Node *Layer;
typedef std::vector<Layer> SkipList;

Node *add_node_to_layer(Layer &lyr, int num, Node *bottom) {
    if ((lyr == nullptr) || (lyr->data > num)) {
        Node *tmp = lyr;
        lyr = new Node;
        lyr->next = tmp;
        lyr->bottom = bottom;
        lyr->data = num;
        return lyr;
    } else {
        Node *ptr = lyr;
        while (true) {
            if (ptr->next == nullptr) break;
            else if (ptr->next->data > num) break;
            ptr = ptr->next;
        }
        Node *tmp = ptr->next;
        ptr->next = new Node;
        ptr->next->next = tmp;
        ptr->next->bottom = bottom;
        ptr->next->data = num;
        return ptr->next;
    }
}

void add_node_to_skiplist(SkipList &lst, int num) {
    int lyr_num = 0;
    double rand = rand_num(10);
    Node *ptr = add_node_to_layer(lst[0], num, nullptr);
    while (rand < chance) {
        lyr_num++;
        if (lyr_num + 1 > lst.size()) lst.push_back(nullptr);
        ptr = add_node_to_layer(lst[lyr_num], num, ptr);
        rand = rand_num(10);
    }

}

void print_layer(Layer lyr) {
    Node *ptr = lyr;
    while (ptr != nullptr) {
        std::cout << ptr->data << ' ';
        ptr = ptr->next;
    }
    std::cout << std::endl;
}


int main() {
    SkipList lst;
    lst.push_back(nullptr);

    for (int i = 0; i < 20; i++) {
        add_node_to_skiplist(lst, i);
    }


    for (int i = 0; i < lst.size(); i++) {
        print_layer(lst[i]);
    }


    return 0;
}