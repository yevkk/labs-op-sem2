#include <iostream>
#include <vector>

using namespace std;

const int N = 10;
struct Circle {
    struct {
        double x, y;
    } center;
    double radius;
};

struct Node {
    Circle circle;
    Node *prev = nullptr;
    Node *next = nullptr;
};

struct List {
    Node *head = nullptr, *tail = nullptr;
} queue_list;

void create_empty_list(List queue) {
    while (queue.head != queue.tail) {
        queue.tail = queue.tail->next;
        delete queue.tail;
    }
}

void create_empty_array(int queue_size) {
    queue_size = 0;
}

void create_empty_array(vector<Circle> queue) {
    queue.clear();
}

bool is_empty_list(List queue) {
    return (queue.head == nullptr) ? true : false;
}

bool is_empty_list(int queue_size) {
    return (queue_size == 0) ? true : false;
}

bool is_empty_list(vector<Circle> queue) {
    return (queue.size() == 0) ? true : false;
}

int main() {
    struct {
        Circle data[N];
        int size = 0;
    } queue_array;


    vector<Circle> queue_vector;


    return 0;
}