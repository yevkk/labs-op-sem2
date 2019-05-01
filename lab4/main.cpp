// 1, 4, 8, 15, 18, 20, 23
#include <iostream>

//---------- TOOLS ----------


//---------- Unit 0 (task 1) ----------
const int MAX_CH = 10; //max number of children;

struct TreeNode {
    int data;
    int deg = 0;
    TreeNode* children[MAX_CH];

    TreeNode(int Data){
        data = Data;
        for (int i = 0; i < MAX_CH; i++)
            children[i] = nullptr;
    }
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}