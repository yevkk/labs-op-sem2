#include <iostream>
#include <vector>
#include <random>

double rand_num(double max) {
    static std::random_device rd;
    static std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(0, max);

    return dist(mt);
}

const double chance = 0.5;

struct Node {
    int data = (int) NAN;
    Node *next = nullptr;
    Node *bottom = nullptr;
};
typedef Node *Layer;
typedef std::vector<Layer> SkipList;

void add_node_to_layer(Layer &lyr, int num, Node *bottom) {
    if ((lyr == nullptr) || (lyr->data > num)) {
        Node *tmp = lyr;
        lyr = new Node;
        lyr->next = tmp;
        lyr->bottom = bottom;
        lyr->data = num;
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
    }
}

void add_node_to_skiplist(SkipList &lst, int num) {
    Node *ptr = new Node;

}

void print_layer(Layer lyr) {
    Node *ptr = lyr;
    while (ptr != nullptr) {
        std::cout << ptr->data << std::endl;
        ptr = ptr->next;
    }
}


int main() {
    SkipList lst;
    lst.push_back(*(new Layer));
    lst[0] = nullptr;





    return 0;
}