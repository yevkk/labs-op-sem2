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

    explicit ExprTreeNode(std::string Data, ExprTreeNode *Left, ExprTreeNode *Right) {
        data = Data;
        left = Left;
        right = Right;
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
            if (root->threadedLeft) {
                if (root->left != nullptr) {
                    std::cout << " l: " << root->left->data;
                } else {
                    std::cout << " l: -";
                }
            }
            if (root->threadedRight) {
                if (root->right != nullptr) {
                    std::cout << " r: " << root->right->data;
                } else {
                    std::cout << " r: -";
                }
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

//--- for units 4-5 ---
void delete_bt(BinTreeNode *&root) {
    if (root != nullptr) {
        delete_bt(root->left);
        delete_bt(root->right);
        delete root;
        root = nullptr;
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

struct Variable {
    std::string name;
    double val;
};
std::vector<Variable> variables;
std::vector<Variable> variables_demo;

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

//tools
void print_error() {
    std::cout << "EXPRESSION ERROR FOUND (check input and variables)" << std::endl;
}

ExprTreeNode *copy_expr_tree_node(ExprTreeNode *&node) {
    return (node == nullptr) ? nullptr : new ExprTreeNode(node->data, copy_expr_tree_node(node->left),
                                                          copy_expr_tree_node(node->right));;
}

void delete_expr_tree_node(ExprTreeNode *&node) {
    if (node != nullptr) {
        delete_expr_tree_node(node->left);
        delete_expr_tree_node(node->right);
        delete node;
    }
}

void expr_tree_node_reset_children(ExprTreeNode *&node) {
    delete_expr_tree_node(node->left);
    delete_expr_tree_node(node->right);
    node->left = node->right = nullptr;
}

//reading of expression;
std::vector<std::string> read_expr() {
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
ExprTreeNode *add_node_to_expr_tree(std::vector<std::string> &expr, int &index, bool &error) {
    if (index < expr.size() - 1) {
        if (is_double(expr[index])) {
            return new ExprTreeNode(expr[index], nullptr, nullptr);
        } else if (is_unary_operation(expr[index])) {
            return new ExprTreeNode(expr[index], add_node_to_expr_tree(expr, ++index, error), nullptr);
        } else if (is_binary_operation(expr[index])) {
            return new ExprTreeNode(expr[index], add_node_to_expr_tree(expr, ++index, error),
                                    add_node_to_expr_tree(expr, ++index, error));
        } else if (is_variable(expr[index])) {
            bool flag = true;
            for (auto &v : variables)
                if (v.name == expr[index]) {
                    flag = false;
                    break;
                }
            if (flag) variables.push_back({expr[index], NAN});
            return new ExprTreeNode(expr[index], nullptr, nullptr);
        }
        error = true;
    }
    return nullptr;
}

bool create_expr_tree(ExprTreeNode *&root, std::vector<std::string> &src) { //returns false if error found;
    int index = 0;
    bool error = false;
    root = add_node_to_expr_tree(src, index, error);
    if ((index != src.size() - 2) || error) {
        print_error();
        return false;
    }
    return true;
}

//simplifying expression tree;
void simplify_expr_tree_node(ExprTreeNode *&node, bool &error) {
    if (node != nullptr) {
        simplify_expr_tree_node(node->left, error);
        simplify_expr_tree_node(node->right, error);
        if (node->data == "+") {
            if (node->left->data == "0") {
                delete node->left;
                node = node->right;
            } else if (node->right->data == "0") {
                delete node->right;
                node = node->left;
            }
        } else if (node->data == "-") {
            if ((node->left->data == "0") && is_double(node->right->data)) {
                node->data = node->right->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                expr_tree_node_reset_children(node);
            } else if (node->right->data == "0") {
                delete node->left;
                node = node->left;
            }
        } else if (node->data == "*") {
            if ((node->left->data == "0") || (node->right->data == "0")) {
                node->data = "0";
                expr_tree_node_reset_children(node);
            } else if (node->left->data == "1") {
                delete node->left;
                node = node->right;
            } else if (node->right->data == "1") {
                delete node->right;
                node = node->left;
            } else if ((node->left->data == "-1") && is_double(node->right->data)) {
                node->data = node->right->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                expr_tree_node_reset_children(node);
            } else if ((node->right->data == "-1") && is_double(node->left->data)) {
                node->data = node->left->data;
                if (node->data[0] == '-') node->data.erase(0);
                else node->data = '-' + node->data;
                expr_tree_node_reset_children(node);
            }
        } else if (node->data == "/") {
            if (node->left->data == "0") {
                node->data = "0";
                expr_tree_node_reset_children(node);
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
                    expr_tree_node_reset_children(node);
                }
            } else if (node->right->data == "0") {
                node->data = "1";
                expr_tree_node_reset_children(node);
            } else if (node->left->data == "1") {
                node->data = "1";
                expr_tree_node_reset_children(node);
            } else if (node->right->data == "1") {
                delete node->right;
                node = node->left;
            }
        } else if (node->data == "ln") {
            if ((is_double(node->left->data)) && (std::strtod(node->left->data.c_str(), nullptr)) <= 0) {
                error = true;
                return;
            } else if (node->left->data == "1") {
                node->data = "0";
                expr_tree_node_reset_children(node);
            }
        }
    }
}

bool simplify_expr_tree(ExprTreeNode *&root) { //returns false if error found;
    bool error = false;
    simplify_expr_tree_node(root, error);
    if (error) print_error();
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
                    expr_tree_node_reset_children(node);
                } else if (node->data == "-") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) - strtod(node->right->data.c_str(), nullptr));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "*") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) * strtod(node->right->data.c_str(), nullptr));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "/") {
                    node->data = std::to_string(
                            strtod(node->left->data.c_str(), nullptr) / strtod(node->right->data.c_str(), nullptr));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "^") {
                    node->data = std::to_string(
                            pow(strtod(node->left->data.c_str(), nullptr), strtod(node->right->data.c_str(), nullptr)));
                    expr_tree_node_reset_children(node);
                }
            }
        } else if (is_unary_operation(node->data)) {
            if (is_double(node->left->data)) {
                if (node->data == "sin") {
                    node->data = std::to_string(sin(strtod(node->left->data.c_str(), nullptr)));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "cos") {
                    node->data = std::to_string(cos(strtod(node->left->data.c_str(), nullptr)));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "tan") {
                    node->data = std::to_string(tan(strtod(node->left->data.c_str(), nullptr)));
                    expr_tree_node_reset_children(node);
                } else if (node->data == "ln") {
                    double d = strtod(node->left->data.c_str(), nullptr);
                    if (d > 0) node->data = std::to_string(log(d));
                    else {
                        error = true;
                        return;
                    }
                    expr_tree_node_reset_children(node);
                }
            }
        }
    }
}

bool calculate_constants_expr_tree(ExprTreeNode *&root) { //returns false if error found;
    bool error = false;
    simplify_expr_tree_node(root, error); //for catching expression errors;
    if (!error) calculate_constants_tree_node(root, error);
    if (error) print_error();
    return !error;
}

//calculation expression result
void variable_value_substitution(ExprTreeNode *&node, bool demo = false) {
    if (node != nullptr) {
        variable_value_substitution(node->left, demo);
        variable_value_substitution(node->right, demo);
        if (demo) {
            for (auto &v:variables_demo)
                if (node->data == v.name) {
                    node->data = std::to_string(v.val);
                    return;
                }
        } else {
            for (auto &v:variables)
                if (node->data == v.name) {
                    node->data = std::to_string(v.val);
                    return;
                }
        }
    }
}

double expr_tree_result(ExprTreeNode *root, bool demo = false) {
    ExprTreeNode *new_root = copy_expr_tree_node(root);
    if (demo) {
        for (auto &v:variables_demo) {
            v.val = rand_double(-10, 10);
            std::cout << v.name << " = " << v.val << std::endl;
        }
    } else {
        if (variables.empty()) std::cout << "Enter values:" << std::endl;
        for (auto &v:variables) {
            std::cout << v.name << " = ";
            std::cin >> v.val;
        }
    }
    variable_value_substitution(new_root, demo);
    print_tree(new_root);
    double result = (calculate_constants_expr_tree(new_root)) ? std::strtod(new_root->data.c_str(), nullptr)
                                                              : NAN;
    delete_expr_tree_node(new_root);
    return result;
}

//derivation
ExprTreeNode *expr_tree_derivation(ExprTreeNode *node, std::string var) {
    if (node->data == var)
        return new ExprTreeNode("1", nullptr, nullptr);
    else if (node->data == "+")
        return new ExprTreeNode("+", expr_tree_derivation(node->left, var),
                                expr_tree_derivation(node->right, var));
    else if (node->data == "-")
        return new ExprTreeNode("-", expr_tree_derivation(node->left, var),
                                expr_tree_derivation(node->right, var));
    else if (node->data == "*")
        return new ExprTreeNode("+",
                                new ExprTreeNode("*", expr_tree_derivation(node->left, var),
                                                 copy_expr_tree_node(node->right)),
                                new ExprTreeNode("*", copy_expr_tree_node(node->left),
                                                 expr_tree_derivation(node->right, var))
        );
    else if (node->data == "/")
        return new ExprTreeNode("/",
                                new ExprTreeNode("-",
                                                 new ExprTreeNode("*", expr_tree_derivation(node->left, var),
                                                                  copy_expr_tree_node(node->right)),
                                                 new ExprTreeNode("*", copy_expr_tree_node(node->left),
                                                                  expr_tree_derivation(node->right, var))),
                                new ExprTreeNode("^", copy_expr_tree_node(node->right),
                                                 new ExprTreeNode("2", nullptr, nullptr))
        );
    else if (node->data == "sin")
        return new ExprTreeNode("*",
                                new ExprTreeNode("cos", copy_expr_tree_node(node->left), nullptr),
                                expr_tree_derivation(node->left, var)
        );
    else if (node->data == "cos")
        return new ExprTreeNode("*",
                                new ExprTreeNode("*",
                                                 new ExprTreeNode("sin", copy_expr_tree_node(node->left),
                                                                  nullptr),
                                                 new ExprTreeNode("-1", nullptr, nullptr)
                                ),
                                expr_tree_derivation(node->left, var)
        );
    else if (node->data == "ln")
        return new ExprTreeNode("*",
                                new ExprTreeNode("/", new ExprTreeNode("1", nullptr, nullptr),
                                                 copy_expr_tree_node(node->left)),
                                expr_tree_derivation(node->left, var)
        );
    else if (node->data == "tan")
        return new ExprTreeNode("*",
                                new ExprTreeNode("/",
                                                 new ExprTreeNode("1", nullptr, nullptr),
                                                 new ExprTreeNode("^",
                                                                  new ExprTreeNode("cos",
                                                                                   copy_expr_tree_node(node->left),
                                                                                   nullptr),
                                                                  new ExprTreeNode("2", nullptr, nullptr)
                                                 )
                                ),
                                expr_tree_derivation(node->left, var)
        );
    else if (node->data == "^")
        return new ExprTreeNode("*",
                                new ExprTreeNode("+",
                                                 new ExprTreeNode("*",
                                                                  new ExprTreeNode("ln",
                                                                                   copy_expr_tree_node(node->left),
                                                                                   nullptr),
                                                                  expr_tree_derivation(node->right, var)
                                                 ),
                                                 new ExprTreeNode("/",
                                                                  new ExprTreeNode("*",
                                                                                   expr_tree_derivation(node->left,
                                                                                                        var),
                                                                                   copy_expr_tree_node(node->right)
                                                                  ),
                                                                  copy_expr_tree_node(node->left)
                                                 )
                                ),
                                new ExprTreeNode("^",
                                                 copy_expr_tree_node(node->left),
                                                 copy_expr_tree_node(node->right)
                                )
        );
    else return new ExprTreeNode("0", nullptr, nullptr);
}

//---------- DEMO ----------
void demo_tree() {
    TreeNode *root = nullptr;
    double p;
    int val, num = rand_int(7, 15);
    std::cout << "=============== TREE DEMO ===============" << std::endl << std::endl;

    std::cout << "ADDING ELEMENTS" << std::endl;
    for (int i = 0; i < num; i++) {
        p = rand_double(0, 1);
        val = rand_int(-10, 10);
        std::cout << "Value = " << val << ", p = " << p << std::endl;
        add_node_to_tree(root, val, p);
        print_tree(root);
        std::cout << std::endl;
    }

    num = rand_int(4, 8);
    std::cout << "DELETING ELEMENTS" << std::endl;
    for (int i = 0; i < num; i++) {
        val = rand_int(-7, 7);
        std::cout << "Value = " << val << std::endl;
        delete_nodes_from_tree(root, val);
        print_tree(root);
        std::cout << std::endl;
    }
    std::cout << "=========================================" << std::endl << std::endl;
}

void demo_binary_tree() {
    BinTreeNode *root = nullptr;
    int val, num = rand_int(10, 20);
    std::cout << "=============== BINARY TREE DEMO ===============" << std::endl << std::endl;

    std::cout << "ADDING ELEMENTS" << std::endl;
    for (int i = 0; i < num; i++) {
        val = rand_int(-15, 15);
        std::cout << "Value = " << val << std::endl;
        add_node_to_bt(root, val);
        print_tree(root);
        std::cout << std::endl;
    }

    std::cout << "CONVERTING BINARY TREE TO THREADED" << std::endl;
    bt_to_threaded(root);
    print_tree(root);
    std::cout << std::endl;

    std::cout << "================================================" << std::endl;
}

void expression_generator(std::vector<std::string> &result, int step = 0) {
    std::string var[] = {"a"};
    std::string un_op[] = {"sin", "cos", "tan", "ln"};
    std::string bin_op[] = {"+", "-", "*", "/", "^"};
    if ((rand_double(0, 1) < 0.5) || (step <= 3)) {
        if (rand_double(0, 1) < 0.5) {
            result.push_back(un_op[rand_int(0, un_op->size() - 1)]);
            expression_generator(result, ++step);
        } else {
            result.push_back(bin_op[rand_int(0, bin_op->size() - 1)]);
            expression_generator(result, ++step);
            expression_generator(result, ++step);
        }
    } else {
        if (rand_double(0, 1) < 0.5) {
            result.push_back(var[rand_int(0, var->size() - 1)]);
        } else {
            result.push_back(std::to_string(rand_double(-10, 10)));
        }
    }
}

std::vector<std::string> random_expression() {
    std::vector<std::string> result;
    expression_generator(result);
    result.emplace_back("//");
    return result;
}

void demo_expression_tree() {
    ExprTreeNode *root = nullptr;
    variables_demo.push_back({"a", 0});
    std::vector<std::string> expression = random_expression();
    std::cout << "=============== EXPRESSION TREE DEMO ===============" << std::endl << std::endl;
    for (auto &s:expression)
        std::cout << s << "  ";
    std::cout << std::endl << std::endl;

    create_expr_tree(root, expression);
    print_tree(root);

    std::cout << "SIMPLIFYING" << std::endl;
    simplify_expr_tree(root);
    print_tree(root);
    std::cout << std::endl;

    std::cout << "CONSTANT CALCULATION" << std::endl;
    calculate_constants_expr_tree(root);
    print_tree(root);
    std::cout << std::endl;

    double res;
    std::cout << "RESULTS CALCULATION" << std::endl;
    for (int i = 1; i <= 2; i++) {
        std::cout << "Example #" << i << std::endl;
        res = expr_tree_result(root, true);
        std::cout << std::endl << "Result: " << res << std::endl << std::endl;
    }

    std::cout << "DERIVATION BY " << variables_demo[0].name << std::endl;
    root = expr_tree_derivation(root, variables_demo[0].name);
    print_tree(root);
    std::cout << std::endl;

    variables_demo.clear();
    std::cout << "================================================" << std::endl;
}

//---------- INTERACTIVE ----------
void interactive_tree() {
    static TreeNode *root = nullptr;
    int command;
    int val;
    double p;
    while (true) {
        command = 0;
        std::cout
                << "\nEnter command:\n"
                   "\t#1 - add node;\n"
                   "\t#2 - delete nodes by value;\n"
                   "\t#3 - print\n"
                   "\t#4 - demo\n"
                   "#5 - change type\n";
        while ((command < 1) || (command > 5)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 1: {
                std::cout << "value = ";
                std::cin >> val;
                std::cout << "p = ";
                std::cin >> p;
                add_node_to_tree(root, val, p);
                break;
            }
            case 2: {
                std::cout << "value = ";
                std::cin >> val;
                delete_nodes_from_tree(root, val);
                break;
            }
            case 3: {
                if (root != nullptr)
                    print_tree(root);
                else std::cout << "Tree is empty" << std::endl;
                break;
            }
            case 4: {
                demo_tree();
                break;
            }
            case 5: {
                delete_nodes_from_tree(root, root->data);
                return;
            }
            default:;
        }
    }
}

void interactive_binary_tree() {
    static BinTreeNode *root = nullptr;
    int command;
    int val;
    bool threaded = false;
    while (true) {
        command = 0;
        std::cout
                << "\nEnter command:\n"
                   "\t#1 - add node;\n"
                   "\t#2 - transform tree to threaded;\n"
                   "\t#3 - transform threaded to normal;\n"
                   "\t#4 - print\n"
                   "\t#5 - demo\n"
                   "#6 - change type\n";
        while ((command < 1) || (command > 6)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 1: {
                std::cout << "value = ";
                std::cin >> val;
                add_node_to_bt(root, val);
                break;
            }
            case 2: {
                if (threaded) {
                    std::cout << "Tree is already threaded";
                } else {
                    threaded = true;
                    bt_to_threaded(root);
                }
                break;
            }
            case 3: {
                if (!threaded) {
                    std::cout << "Tree is already normal";
                } else {
                    threaded = false;
                    bt_to_threaded_reverse(root);
                }
                break;
            }
            case 4: {
                if (root != nullptr)
                    print_tree(root);
                else std::cout << "Tree is empty" << std::endl;
                break;
            }
            case 5: {
                demo_binary_tree();
                break;
            }
            case 6: {
                delete_bt(root);
                return;
            }
            default:;
        }
    }
}

void interactive_expression_tree() {
    static ExprTreeNode *root = nullptr;
    std::vector<std::string> expression;
    int command;
    double res;
    std::string var;
    std::cout << "Enter expression (prefix notation, enter \"//\" to indicate end):\n";
    expression = read_expr();
    bool error = create_expr_tree(root, expression);

    while (true) {
        if (!error) {
            //print_error();
            return;
        }
        command = 0;
        std::cout
                << "\nEnter command:\n"
                   "\t#1 - simplify expression;\n"
                   "\t#2 - calculate constants;\n"
                   "\t#3 - calculate result;\n"
                   "\t#4 - derivation\n"
                   "\t#5 - print\n"
                   "\t#6 - demo\n"
                   "#7 - change type\n";
        while ((command < 1) || (command > 6)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 1: {
                error = simplify_expr_tree(root);
                if (error) std::cout << "Command completed successfully\n";
                break;
            }
            case 2: {
                error = calculate_constants_expr_tree(root);
                if (error) std::cout << "Command completed successfully\n";
                break;
            }
            case 3: {
                res = expr_tree_result(root);
                std::cout << "Result = " << res << std::endl;
                if (error) std::cout << "Command completed successfully\n";
                break;
            }
            case 4: {

                std::cout << "by variable:";
                std::cin >> var;
                root = expr_tree_derivation(root, var);
                if (error) std::cout << "Command completed successfully\n";
                break;
            }
            case 5: {
                print_tree(root);
                break;
            }
            case 6: {
                demo_expression_tree();
                break;
            }
            case 7: {
                variables.clear();
                delete_expr_tree_node(root);
                return;
            }
            default:;
        }
    }
}

int main() {
    int command;
    while (true) {
        command = 0;
        std::cout << "Enter type of tree:\n\t#1 - tree;\n\t#2 - binary tree;\n\t#3 - expression tree\n#4 - stop\n";
        while ((command < 1) || (command > 4)) {
            std::cout << "\n#";
            std::cin >> command;
        }
        switch (command) {
            case 1: {
                interactive_tree();
                break;
            }
            case 2: {
                interactive_binary_tree();
                break;
            }
            case 3: {
                interactive_expression_tree();
                break;
            }
            case 4: {
                return 0;
            }
            default:;
        }
    }
}