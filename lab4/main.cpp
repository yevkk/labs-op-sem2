// 1, 4, 8, 15, 18, 20, 23
#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <vector>
#include <cmath>

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

//--- for units 4-5 ---
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

//--- for unit 6 ---
struct ExprTreeNode {
    std::string data;
    ExprTreeNode *left, *right;

    explicit ExprTreeNode(std::string &Data) {
        data = Data;
        left = right = nullptr;
    }

    void reset_children() {
        delete left;
        delete right;
        left = nullptr;
        right = nullptr;
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

//--- for units 4-5 ---
void print_tree(BinTreeNode *root, int level = 0) {
    if (!level) std::cout << "Binary tree:" << std::endl;
    if (root != nullptr) {
        std::cout << '|';
        for (int i = 0; i < level; i++)
            std::cout << '\t' << '|';
        std::cout << root->data;
        if (root->threadedLeft || root->threadedRight) {
            std::cout << "  threads:";
            if (root->threadedLeft)
                if (root->left != nullptr) {
                    std::cout << " l: " << root->left->data;
                } else {
                    std::cout << " l: -";
                }
            if (root->threadedRight)
                if (root->right != nullptr) {
                    std::cout << " r: " << root->right->data;
                } else {
                    std::cout << " r: -";
                }
        }
        std::cout << std::endl;
        if (!root->threadedLeft) print_tree(root->left, level + 1);
        if (!root->threadedRight) print_tree(root->right, level + 1);
    }
}

//--- for unit 6 ---
void print_tree(ExprTreeNode *root, int level = 0) {
    if (!level) std::cout << "Expression tree:" << std::endl;
    if (root != nullptr) {
        std::cout << '|';
        for (int i = 0; i < level; i++)
            std::cout << '\t' << '|';
        std::cout << root->data << std::endl;
        print_tree(root->left, level + 1);
        print_tree(root->right, level + 1);
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
void create_sym_traversal(BinTreeNode *node, std::vector<BinTreeNode *> &res) {
    if (node != nullptr) {
        create_sym_traversal(node->left, res);
        res.push_back(node);
        create_sym_traversal(node->right, res);
    }
};

void bt_to_threaded(BinTreeNode *&root) {
    std::vector<BinTreeNode *> sym_traversal;
    std::cout << std::endl;
    create_sym_traversal(root, sym_traversal);
    for (int i = 0; i < sym_traversal.size(); i++) {
        if (sym_traversal[i]->left == nullptr) {
            sym_traversal[i]->threadedLeft = true;
            if (i != 0)sym_traversal[i]->left = sym_traversal[i - 1];
        }
        if (sym_traversal[i]->right == nullptr) {
            sym_traversal[i]->threadedRight = true;
            if (i != sym_traversal.size() - 1)sym_traversal[i]->right = sym_traversal[i + 1];
        }
    }
}

void bt_to_threaded_reverse(BinTreeNode *&root) {
    if (root != nullptr) {
        if (root->threadedLeft) root->left = nullptr;
        if (root->threadedRight) root->right = nullptr;
        root->threadedLeft = root->threadedRight = false;
        bt_to_threaded_reverse(root->left);
        bt_to_threaded_reverse(root->right);
    }
}

//---------- UNIT 6 (task 23) ----------
std::string EXPR_END = "//";
std::string ERROR_MSG = "EXPRESSION ERROR FOUND (check input)";

struct Variable {
    std::string name;
    double val;
};
std::vector<Variable> variables;


//defining type of string;
bool is_double(std::string &str) {
    int point = 0;
    if (str == "-") return false;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == '-') {
            if (i == 0)
                continue;
            else
                return false;
        }
        if (str[i] == '.') {
            point++;
            if (point > 1) return false;
            continue;
        }
        if ((str[i] < '0') || (str[i] > '9')) return false;
    }
    return true;
}

bool is_variable(std::string &str) {
    for (int i = 0; i < str.size(); i++) {
        if (((str[i] >= 'a') && (str[i] <= 'z')) || ((str[i] >= 'A') && (str[i] <= 'Z'))) continue;
        if (((str[i] >= '0') && (str[i] <= '9')) && (i != 0)) continue;
        return false;
    }
    return true;
}

bool is_binary_operation(std::string &str) {
    std::string binaryOperations[] = {"+", "-", "*", "/", "^"};
    for (auto &s:binaryOperations)
        if (str == s) return true;
    return false;
}

bool is_unary_operation(std::string &str) {
    std::string unaryOperations[] = {"sin", "cos", "tan", "ln"};
    for (auto &s:unaryOperations)
        if (str == s) return true;
    return false;
}

//reading of expression;
std::vector<std::string> read_expression() {
    std::vector<std::string> res;
    std::string str = " ";
    std::cout << "Enter expression (prefix notation, enter // to mark the end of expression):" << std::endl;
    while (str != EXPR_END) {
        std::cin >> str;
        res.push_back(str);
    }
    return res;
}

//transforming expression to expression tree;
void add_node_to_expression_tree(ExprTreeNode *&node, std::vector<std::string> &expr, int &index, bool &error) {
    if (index < expr.size() - 1) {
        if (is_double(expr[index])) {
            node = new ExprTreeNode(expr[index]);
            return;
        } else if (is_unary_operation(expr[index])) {
            node = new ExprTreeNode(expr[index]);
            add_node_to_expression_tree(node->left, expr, ++index, error);
            return;
        } else if (is_binary_operation(expr[index])) {
            node = new ExprTreeNode(expr[index]);
            add_node_to_expression_tree(node->left, expr, ++index, error);
            add_node_to_expression_tree(node->right, expr, ++index, error);
            if ((node->data == "/") && (node->right->data == "0")) error = true;
            return;
        } else if (is_variable(expr[index])) {
            variables.push_back({expr[index], 0});
            node = new ExprTreeNode(expr[index]);
            return;
        }
    }
    error = true;
}

bool create_expression_tree(ExprTreeNode *&root, std::vector<std::string> &src) { //returns false if error found;
    int index = 0;
    bool error = false;
    add_node_to_expression_tree(root, src, index, error);
    if (error || (index != src.size() - 2)) std::cout << ERROR_MSG << std::endl;
    return !error;
}

//simplifying expression tree;
void simplify_expression_tree_node(ExprTreeNode *&node, bool &error) {
    if (node != nullptr) {
        simplify_expression_tree_node(node->left, error);
        simplify_expression_tree_node(node->right, error);
        if (node->data == "+") {
            if (node->left->data == "0") {
                node->data = node->right->data;
                node->reset_children();
            } else if (node->right->data == "0") {
                node->data = node->left->data;
                node->reset_children();
            }
        } else if (node->data == "-") {
            if ((node->left->data == "0") && is_double(node->right->data)) {
                node->data = node->right->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                node->reset_children();
            } else if (node->right->data == "0") {
                node->data = node->left->data;
                node->reset_children();
            }
        } else if (node->data == "*") {
            if ((node->left->data == "0") || (node->right->data == "0")) {
                node->data = "0";
                node->reset_children();
            } else if (node->left->data == "1") {
                node->data = node->right->data;
                node->reset_children();
            } else if (node->right->data == "1") {
                node->data = node->left->data;
                node->reset_children();
            } else if ((node->left->data == "-1") && is_double(node->right->data)) {
                node->data = node->right->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                node->reset_children();
            } else if ((node->right->data == "-1") && is_double(node->left->data)) {
                node->data = node->left->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                node->reset_children();
            }
        } else if (node->data == "/") {
            if (node->left->data == "0") {
                node->data = "0";
                node->reset_children();
            } else if (node->right->data == "0") {
                error = true;
                return;
            }
        } else if (node->data == "^") {
            if (node->left->data == "0") {
                if ((node->right->data[0] == '-') || (node->right->data == "0")) {
                    error = true;
                    return;
                } else {
                    node->data = "0";
                    node->reset_children();
                }
            } else if (node->right->data == "0") {
                node->data = "1";
                node->reset_children();
            } else if (node->left->data == "1") {
                node->data = "1";
                node->reset_children();
            } else if (node->right->data == "1") {
                node->data = node->left->data;
                node->reset_children();
            }
        } else if (node->data == "ln") {
            if (node->left->data == "0") {
                error = true;
                return;
            } else if (node->left->data[0] == '1') {
                node->data = "0";
                node->reset_children();
            }
        }
    }
}

bool simplify_expression_tree(ExprTreeNode *&root) { //returns false if error found;
    bool error = false;
    simplify_expression_tree_node(root, error);
    if (error) std::cout << ERROR_MSG << std::endl;
    return !error;
}

//calculation of constants
void calculate_constants_tree_node(ExprTreeNode *&node, bool &error) {
    using std::strtod;
    if (node != nullptr) {
        calculate_constants_tree_node(node->left, error);
        calculate_constants_tree_node(node->right, error);

        if (is_binary_operation(node->data)) {
            if (is_double(node->left->data) && is_double(node->right->data)) {
                if (node->data == "+") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) + strtod(node->right->data.c_str(), nullptr));
                    node->reset_children();
                } else if (node->data == "-") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) - strtod(node->right->data.c_str(), nullptr));
                    node->reset_children();
                } else if (node->data == "*") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) * strtod(node->right->data.c_str(), nullptr));
                    node->reset_children();
                } else if (node->data == "/") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) / strtod(node->right->data.c_str(), nullptr));
                    node->reset_children();
                } else if (node->data == "^") {
                    node->data = std::to_string(
                            pow(strtod(node->left->data.c_str(), nullptr), strtod(node->right->data.c_str(), nullptr)));
                    node->reset_children();
                }
            }
        } else if (is_unary_operation(node->data)) {
            if (is_double(node->left->data)) {
                if (node->data == "sin") {
                    node->data = std::to_string(sin(strtod(node->left->data.c_str(), nullptr)));
                    node->reset_children();
                } else if (node->data == "cos") {
                    node->data = std::to_string(cos(strtod(node->left->data.c_str(), nullptr)));
                    node->reset_children();
                } else if (node->data == "tan") {
                    node->data = std::to_string(tan(strtod(node->left->data.c_str(), nullptr)));
                    node->reset_children();
                } else if (node->data == "ln") {
                    double d = strtod(node->left->data.c_str(), nullptr);
                    if (d > 0) node->data = std::to_string(log(d));
                    else {
                        error = true;
                        return;
                    }
                    node->reset_children();
                }
            }
        }
    }
}

bool calculate_constants_expression_tree(ExprTreeNode *&root) { //returns false if error found;
    bool error = false;
    simplify_expression_tree_node(root, error); //for catching expression errors;
    if (!error) calculate_constants_tree_node(root, error);
    if (error) std::cout << ERROR_MSG << std::endl;
    return !error;
}


int main() {
//    BinTreeNode *root = nullptr;
//    for (int i = 0; i < 30; i++) {
//        add_node_to_bt(root, rand_int(0, MAX_VAL));
//    }
//    print_tree(root);
//    std::cout << std::endl << std::endl;
//    bt_to_threaded(root);
//    print_tree(root);
//    std::cout << std::endl << std::endl;
//    bt_to_threaded_reverse(root);
//    print_tree(root);
//    std::vector<std::string> expr = read_expression();
//    std::string str = expr.back();
//    ;
    ExprTreeNode *root = nullptr;
    std::vector<std::string> expr = read_expression();
    create_expression_tree(root, expr);
    print_tree(root);
    simplify_expression_tree(root);
    print_tree(root);
    calculate_constants_expression_tree(root);
    print_tree(root);
    return 0;
}