// 1, 4, 8, 15, 18, 20, 23
#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <list>

//---------- TOOLS ----------

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

//---------- UNIT 0 (task 1) ----------
const int MAX_CH = 10; //max number of children;

struct TreeNode {
    int data;
    int deg = 0;
    TreeNode *children[MAX_CH] = {};

    explicit TreeNode(int Data) {
        data = Data;
        for (auto &pt : children)
            pt = nullptr;
    }
};

//--- for units 4-6 ---
struct BinTreeNode {
    int data;
    BinTreeNode *left, *right;
    bool threadedLeft, threadedRight;

    explicit BinTreeNode(int Data) {
        data = Data;
        left = nullptr;
        right = nullptr;
        threadedLeft = false;
        threadedRight = false;
    }
};

//---------- UNIT 1 (task 4) ----------
const int MAX_VAL = 100;

void add_node_to_tree(TreeNode *&root, int val, double p) {
    double p1 = rand_double(0, 1);
    if (root == nullptr) {
        root = new TreeNode(val);
    } else {
        if ((root->deg < MAX_CH) && ((p1 <= p) || (root->deg == 0))) {
            root->children[root->deg] = new TreeNode(val);
            root->deg++;
        } else {
            add_node_to_tree(root->children[rand_int(0, root->deg - 1)], val, p);
        }
    }
}

//---------- UNIT 2 (task 8) ----------
void print_tree(TreeNode *root, int level = 0) {
    if (!level) std::cout << "Tree:" << std::endl;
    if (root != nullptr) {
        std::cout << '|';
        for (int i = 0; i < level; i++)
            std::cout << '\t' << '|';
        std::cout << root->data << std::endl;
        for (auto ch: root->children)
            print_tree(ch, level + 1);
    }
}

//--- for units 4-6 ---
void print_tree(BinTreeNode *root, int level = 0) {
    if (!level) std::cout << "Binary tree:" << std::endl;
    if (root != nullptr) {
        std::cout << '|';
        for (int i = 0; i < level; i++)
            std::cout << '\t' << '|';
        std::cout << root->data;
        if (root->threadedLeft || root->threadedRight) {
            std::cout << "  threaded:";
            if (root->threadedLeft)
                std::cout << " l: " << root->left->data;
            if (root->threadedRight)
                std::cout << " r: " << root->right->data;
        }
        std::cout << std::endl;
        if (!root->threadedLeft) print_tree(root->left, level + 1);
        if (!root->threadedRight) print_tree(root->right, level + 1);
    }
}

//---------- UNIT 3 (task 15) ----------
void delete_children(TreeNode *&node) {
    if (node != nullptr) {
        for (auto &ch: node->children) {
            delete_children(ch);
            ch = nullptr;
        }
    }
}

void delete_nodes_from_tree(TreeNode *&root, int val) {
    if (root != nullptr) {
        if (root->data == val) {
            delete_children(root);
            delete root;
            root = nullptr;
        } else {
            for (auto &ch:root->children) {
                delete_nodes_from_tree(ch, val);
            }
            for (int i = 0; i < root->deg - 1; i++) {
                if (root->children[i] == nullptr) {
                    root->children[i] = root->children[root->deg - 1];
                    root->children[root->deg - 1] = nullptr;
                    root->deg--;
                }
            }
        }
    }

}

//---------- UNIT 4 (task 15) ----------
void add_node_to_bt(BinTreeNode *&root, int val) {
    if (root == nullptr)
        root = new BinTreeNode(val);
    else {
        if (root->data < val) {
            add_node_to_bt(root->right, val);
            return;
        }
        if (root->data > val) {
            add_node_to_bt(root->left, val);
            return;
        }
    }
}

//---------- UNIT 5 (task 20) ----------
//Threaded

int main() {
    BinTreeNode *root = nullptr;
    for (int i = 0; i < 25; i++) {
        add_node_to_bt(root, rand_int(-MAX_VAL, MAX_VAL));
    }
    print_tree(root);
    return 0;
}