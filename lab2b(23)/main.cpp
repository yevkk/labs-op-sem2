#include <iostream>
#include <vector>

const double chance = 0.5;

struct Node{
    int data = NULL;
    Node* next = nullptr;
    Node* bottom = nullptr;
};

typedef Node* Level;
typedef std::vector<Level> SkipList;



int main() {
    SkipList lst;

    return 0;
}