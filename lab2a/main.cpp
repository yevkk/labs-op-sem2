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
    Node *next = nullptr;
};

struct List {
    Node *head = nullptr, *tail = nullptr;
};

void create_empty(List &queue) {
    while (queue.head != queue.tail) {
        queue.tail = queue.tail->next;
        delete queue.tail;
    }
}

void create_empty(int &queue_size) {
    queue_size = 0;
}

void create_empty(vector<Circle> &queue) {
    queue.clear();
}

bool is_queue_empty(List queue) {
    return (queue.head == nullptr);
}

bool is_queue_empty(int queue_size) {
    return (queue_size == 0);
}

bool is_queue_empty(vector<Circle> queue) {
    return (queue.size() == 0);
}

void enqueue(List &queue, double x, double y, double radius) {
    Node *ptr = new Node;
    ptr->circle.center.x = x;
    ptr->circle.center.y = y;
    ptr->circle.radius = radius;
    ptr->next = nullptr;
    if (queue.head == nullptr) {
        queue.head = ptr;
        queue.tail = ptr;
    }
    else {
        queue.tail->next = ptr;
        queue.tail = ptr;
    }
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

void enqueue(vector<Circle> &queue, double x, double y, double radius) {
    Circle circle = {x, y, radius};
    queue.push_back(circle);
}

void dequeue(List &queue) {
    if (is_queue_empty(queue)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    Node *ptr = queue.head->next;
    delete queue.head;
    queue.head = ptr;
}

void dequeue(Circle *queue_data, int &queue_size) {
    if (is_queue_empty(queue_size)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    for (int i = 0; i < queue_size - 1; i++) {
        queue_data[i] = queue_data[i + 1];
    }
    queue_size--;
}

void dequeue(vector<Circle> &queue) {
    if (is_queue_empty(queue)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    queue.erase(queue.begin());
}

int main() {
    struct {
        Circle data[MAX_SIZE];
        int size = 0;
    } queue_array;

    vector<Circle> queue_vector;
    List queue_list;
    double x, y, r;

//    create_empty(queue_vector);
//    create_empty(queue_list);
//    create_empty(queue_array.size);
//
//    enqueue(queue_list, 1, 1, 1);
//    enqueue(queue_vector, 1, 1, 1);
//    enqueue(queue_array.data, queue_array.size, 1, 1, 1);
//
//    enqueue(queue_list, 2, 2, 2);
//    enqueue(queue_list, 3, 3, 3);
//    enqueue(queue_vector, 2, 2, 2);
//    enqueue(queue_array.data, queue_array.size, 2, 2, 2);
//
//    dequeue(queue_list);
//    dequeue(queue_vector);
//    dequeue(queue_vector);
//    dequeue(queue_vector);
//    dequeue(queue_array.data, queue_array.size);


    return 0;
}