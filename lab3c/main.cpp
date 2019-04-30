#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <functional>
#include <algorithm>

const int NUM = 100;   //max number of elements in array;
const int NUM1 = 500000; //number of elements in array for benchmark;
const int MAX = 100;   //max value;

//---------- TOOLS ----------

int rand_num(int min, int max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

void set_element(int *arr, int pos) {
    arr[pos] = rand_num(0, MAX);
}

void print_array(int *arr, int &size) {
    std::cout << "Array:\n";
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}

//---------- BINARY TREE SORT ----------

struct BinNode {
    int val;
    int amount = 0;
    BinNode *left = nullptr,
            *right = nullptr;

    BinNode(int Val) {
        val = Val;
        amount = 1;
    }
};
//bt - binary tree;

void add_to_bt(BinNode *&root, int val) {
    if (root == nullptr)
        root = new BinNode(val);
    else {
        if (root->val == val) {
            root->amount++;
            return;
        }
        if (root->val < val) {
            add_to_bt(root->right, val);
            return;
        }
        if (root->val > val) {
            add_to_bt(root->left, val);
            return;
        }
    }
}

void print_tree(BinNode *node) {
    if (node != nullptr) {
        std::cout << node->val << " x" << node->amount;

        std::cout << " left:";
        if (node->left != nullptr)
            std::cout << node->left->val;
        else std::cout << "NAN";

        std::cout << " right:";
        if (node->right != nullptr)
            std::cout << node->right->val;
        else std::cout << "NAN";

        std::cout << std::endl;

        print_tree(node->left);
        print_tree(node->right);
    }
}


void bt_to_array(BinNode *root, int *dst, int &index) {
    if (root == nullptr) return;
    bt_to_array(root->left, dst, index);
    for (int j = 1; j <= root->amount; j++) {
        dst[index] = root->val;
        index++;
    }
    bt_to_array(root->right, dst, index);


}

void bt_sort(int *arr, int size, bool print) {
    BinNode *root = nullptr;
    int k = 0;
    for (int i = 0; i < size; i++) {
        add_to_bt(root, arr[i]);
        if (print) {
            std::cout << i + 1 << ")\n";
            print_tree(root);
        }
    }
    bt_to_array(root, arr, k);

    if (print) {
        std::cout << "\nSorted ";
        print_array(arr, size);
    }
}
//---------- BUCKET SORT ----------

struct ListNode {
    int val;
    ListNode *next = nullptr;

    ListNode(int Val, ListNode *Next) {
        val = Val;
        next = Next;
    }
};

struct SubList {
    double max;
    SubList *next = nullptr;
    ListNode *first = nullptr;

    SubList(double Max, SubList *Next) {
        next = Next;
        max = Max;
    };
};

void print_sublist(SubList *sublist) {
    ListNode *n_pt = sublist->first;
    if (sublist->first == nullptr) return;
    while (n_pt != nullptr) {
        std::cout << n_pt->val << " ";
        n_pt = n_pt->next;
    }
    std::cout << std::endl;
}

void print_list(SubList *list) {
    SubList *sub_pt = list;
    while (sub_pt != nullptr) {
        print_sublist(sub_pt);
        sub_pt = sub_pt->next;
    }
}

void add_to_list(SubList *list, int val) {
    SubList *sub_pt = list;
    ListNode *n_pt;

    while (val > sub_pt->max) {
        sub_pt = sub_pt->next;
    }

    if (sub_pt->first == nullptr) {
        sub_pt->first = new ListNode(val, nullptr);
    } else {
        if (sub_pt->first->val > val) {
            n_pt = new ListNode(val, sub_pt->first);
            sub_pt->first = n_pt;
        } else {
            n_pt = sub_pt->first;
            while (n_pt->next != nullptr) {
                if (n_pt->next->val > val) break;
                n_pt = n_pt->next;
            }
            n_pt->next = new ListNode(val, n_pt->next);
        }
    }
}

void list_to_arr(SubList *list, int *arr) {
    SubList *sub_pt = list;
    ListNode *n_pt;
    int i = 0;

    while (sub_pt != nullptr) {
        n_pt = sub_pt->first;
        while (n_pt != nullptr) {
            arr[i] = n_pt->val;
            i++;
            n_pt = n_pt->next;
        }
        sub_pt = sub_pt->next;
    }
}

void bucket_sort(int *arr, int size, int partition_size, bool print = false) {
    SubList *sub_pt = nullptr;
    int step = 1;
    for (int i = partition_size; i > 0; i--)
        sub_pt = new SubList(((double) MAX * i / partition_size), sub_pt);
    for (int i = 0; i < size; i++) {
        add_to_list(sub_pt, arr[i]);
        if (print) {
            std::cout << std::endl << step++ << ")" << std::endl;
            print_list(sub_pt);
        }
    }
    list_to_arr(sub_pt, arr);
    if (print) print_array(arr, size);

}


//---------- BENCHMARK ----------

template<typename Func>
auto time_ms(const Func &func) {
    using namespace std::chrono;
    const auto begin = system_clock::now();

    func();

    const auto end = system_clock::now();

    return duration_cast<milliseconds>(end - begin).count();
}

void benchmark(int size) {
    int arr[NUM1], *pt;
    int time;

    std::cout << "\nBenchmark (" << size << " elements):\n";

    std::cout << "-std::sort(): ";
    for (int i = 0; i < size; i++) set_element(arr, i);
    time = time_ms([&arr, size]() { std::sort(arr, arr + size); });
    std::cout << time << " ms;\n";

    std::cout << "-Binary Tree Sort: ";
    for (int i = 0; i < size; i++) set_element(arr, i);
    time = time_ms([&arr, &size]() { bt_sort(arr, size, false); });
    std::cout << time << " ms;\n";

    int partition_size = 10;
    size = size / 10;
    std::cout << "-Bucket Sort(partiton size: " << partition_size << ", " << size << " elements): ";
    for (int i = 0; i < size; i++) set_element(arr, i);
    time = time_ms([&arr, &size, partition_size]() { bucket_sort(arr, size, partition_size); });
    std::cout << time << " ms;\n";

}


int main() {
    int arr[NUM];
    int size = 0, command;
    int partition_size;

    for (int i = 0; i < size; i++) {
        set_element(arr, size);
    }

    std::cout << "Array size is set to " << size << std::endl;
    std::cout << "Command list:\n";
    std::cout << "\t1 - Set array size\n";
    std::cout << "\t2 - Demo binary tree sort\n";
    std::cout << "\t3 - Demo bucket sort\n";
    std::cout << "\t4 - Benchmark\n";
    std::cout << "\t5 - Stop\n";

    while (true) {
        std::cout << "\nEnter command number: ";
        std::cin >> command;

        if (command == 5) break;

        switch (command) {
            case 1: {
                size = -1;
                while ((size <= 0) || (size > NUM)) {
                    std::cout << "Enter new size (1-" << NUM << "): ";
                    std::cin >> size;
                }
                for (int i = 0; i < size; i++) {
                    set_element(arr, i);
                }
                std::cout << "Array size is set to " << size << ";\n";
                break;
            }
            case 2: {
                print_array(arr, size);
                if (size != 0) bt_sort(arr, size, true);
                break;
            }
            case 3: {
                print_array(arr, size);
                partition_size = -1;
                while ((partition_size <= 0) || (partition_size > NUM)) {
                    std::cout << "Enter partition size (1-20): ";
                    std::cin >> partition_size;
                }
                if (size != 0) bucket_sort(arr, size, partition_size, true);
                break;
            }
            case 4: {
                benchmark(NUM1);
                break;
            }
            default: {
                break;
            }
        }
    }

    return 0;
}