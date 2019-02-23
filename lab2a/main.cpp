#include <iostream>
#include <vector>

using namespace std;

const int MAX_SIZE = 10;
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

void create_empty(List queue) {
    while (queue.head != queue.tail) {
        queue.tail = queue.tail->next;
        delete queue.tail;
    }
}

void create_empty(int queue_size) {
    queue_size = 0;
}

void create_empty(vector<Circle> queue) {
    queue.clear();
}

bool is_empty(List queue) {
    return (queue.head == nullptr) ? true : false;
}

bool is_empty(int queue_size) {
    return (queue_size == 0) ? true : false;
}

bool is_empty(vector<Circle> queue) {
    return (queue.size() == 0) ? true : false;
}

void enqueue(List queue, double x, double y, double radius) {
    Node *ptr = new Node;
    ptr->next = nullptr;
    ptr->prev = queue.tail;
    ptr->circle.center.x = x;
    ptr->circle.center.y = y;
    ptr->circle.radius = radius;
    queue.tail = ptr;
}

void enqueue(Circle *queue_data, int &queue_size, double x, double y, double radius) {
    if (queue_size + 1 > MAX_SIZE) {
        cout << "element cannot be added, queue is full";
        return;
    }
    queue_data[queue_size].center.x = x;
    queue_data[queue_size].center.y = y;
    queue_data[queue_size].radius = radius;
    queue_size++;
}

void enqueue(vector<Circle> queue, double x, double y, double radius) {
    Circle circle = {x, y, radius};
    queue.push_back(circle);
}

void dequeue(List queue) {
    queue.head = queue.head->next;
    delete queue.head->prev;
    queue.head->prev = nullptr;
}

void dequeue(Circle *queue_data, int &queue_size) {
    for (int i = 0; i < queue_size - 1; i++) {
        queue_data[i] = queue_data[i + 1];
    }
    queue_size--;
}

void dequeue(vector<Circle> queue){
    queue.erase(queue.begin());
}

int main() {
    struct {
        Circle data[MAX_SIZE];
        int size = 0;
    } queue_array;


    vector<Circle> queue_vector;


    return 0;
}