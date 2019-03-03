#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>

double rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_real_distribution<double> dist(0, max);

    return dist(gen);
}

const double CHANCE = 0.5; // (0, 1);

struct Node {
    int data = (int) 0;
    Node *next = nullptr;
    Node *bottom = nullptr;
    int layer = 0;
};
typedef Node *Layer;
typedef std::vector<Layer> SkipList;

Node *search_in_skiplist(SkipList lst, int num, int mode = 1) {
    if (lst[0] == nullptr) return nullptr;
    int lyr_num = lst.size() - 1;
    Node *ptr = lst[lyr_num];

    while (ptr->data > num) {
        ptr = lst[--lyr_num];
        if (mode) std::cout << "| ";
        if (lyr_num <= 0) return nullptr;
    }
    if (mode) std::cout << "- ";

    while (true) {
        while (true) {
            if (ptr->next == nullptr) {
                break;
            } else if (ptr->next->data > num) {
                break;
            }
            if (mode) std::cout << "- ";
            ptr = ptr->next;
        }
        if (ptr->layer == 0) break;

        ptr = ptr->bottom;
        if (mode) std::cout << "| ";
    }
    if (mode) std::cout << std::endl;
    return ((ptr->data == num) ? ptr : nullptr);
}

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

void add_node_to_skiplist(SkipList &lst, int num, int mode = 1) {
    if (search_in_skiplist(lst, num, 0) != nullptr) {
        if (mode) std::cout << "Element " << num << " already exists\n";
        return;
    }

    int lyr_num = 0;
    Node *ptr = add_node_to_layer(lst[0], num, nullptr);
    if (mode) std::cout << "Element " << num << " added to layers: 0 ";

    while (rand_num(1) < CHANCE) {
        lyr_num++;
        if (lyr_num + 1 > lst.size()) lst.push_back(nullptr);
        ptr = add_node_to_layer(lst[lyr_num], num, ptr);
        ptr->layer = lyr_num;
        if (mode) std::cout << lyr_num << ' ';
    }
    if (mode) std::cout << std::endl;

}

void print_layer(Layer lyr) {
    Node *ptr = lyr;
    while (ptr != nullptr) {
        std::cout << ptr->data << ' ';
        ptr = ptr->next;
    }
    std::cout << std::endl;
}

void print_skiplist(SkipList lst) {
    for (int i = lst.size() - 1; i >= 0; i--) {
        std::cout << i << ":  ";
        print_layer(lst[i]);
    }
}

void demonstration(SkipList lst) {
    int num;
    for (int i = 0; i < rand_num(10); i++) {
        num = (int) rand_num(10);
        add_node_to_skiplist(lst, num);
    }

    std::cout << std::endl;
    std::cout << "Print:\n";
    print_skiplist(lst);
    std::cout << std::endl;

    for (int i = 0; i < rand_num(10); i++) {
        num = (int) rand_num(10);
        std::cout << "Search " << num << std::endl;
        std::cout << search_in_skiplist(lst, num) << std::endl << std::endl;
    }
}

int main() {
    SkipList lst;
    lst.push_back(nullptr);
    std::string str;
    int num;

    //for (size_t i = 0; i <= 300; i++) demonstration(lst);

    while (true) {
        std::cout << "\nEnter command: ";
        std::cin >> str;

        if (str == "stop") break;


        if (str == "add") {
            std::cout << "value: ";
            std::cin >> num;
            add_node_to_skiplist(lst, num);
            continue;
        }

        if (str == "print_list") {
            print_skiplist(lst);
            continue;
        }

        if (str == "print_layer") {
            do {
                std::cout << "Layer: ";
                std::cin >> num;
            } while ((num < 0) || (num > lst.size() - 1));
            print_layer(lst[num]);
            continue;
        }

        if (str == "search") {
            std::cout << "value: ";
            std::cin >> num;
            std::cout << search_in_skiplist(lst, num) << std::endl;
            continue;
        }

        if (str == "demo") {
            demonstration(lst);
            continue;
        }

        if (str == "help") {
            std::cout << " - stop\n" << " - add\n" << " - print_list\n" << " - print_layer\n" << " - search\n"
                      << " - demo\n";
            continue;
        }

        std::cout << "Enter correct command (help)\n";
    }

    return 0;
}