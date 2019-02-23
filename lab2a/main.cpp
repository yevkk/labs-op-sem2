#include <iostream>
#include <vector>
#include <string>

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
    } else {
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

void input_values(double &x, double &y, double &radius) {
    cout << "Input:\n" << "  center x:";
    cin >> x;
    cout << "  center y:";
    cin >> y;
    cout << "  radius:";
    cin >> radius;
}

int main() {
    struct {
        Circle data[MAX_SIZE];
        int size = 0;
    } queue_array;

    vector<Circle> queue_vector;
    List queue_list;
    double x, y, r;
    int mode = 0;
    string str;


    while (true) {
        cout << "Enter command: ";
        cin >> str;

        if (str == "stop") break;

        if (str == "select_mode") {
            cout << "1 - list\n2 - array\n3 - vector\n";
            do {
                cin >> mode;
                if ((mode < 1) || (mode > 3)) cout << "select correct mode\n";
            } while ((mode < 1) || (mode > 3));
            continue;
        }

        if (str == "create") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;";
                    break;
                }
                case 1: {
                    create_empty(queue_list);
                    break;
                }
                case 2: {
                    create_empty(queue_array.size);
                    break;
                }
                case 3: {
                    create_empty(queue_vector);
                    break;
                }
            }
            continue;
        }


        if (str == "command_list"){
            cout << "  -stop\n"
                    "  -select_mode\n"
                    "  -demonstrate\n"
                    "  -benchmark\n"
                    "  -create\n"
                    "  -add\n"
                    "  -remove\n"
                    "  -is_empty\n"
                    "  -print\n";
            continue;
        }

        cout << "Enter correct command (help: \"command_list\")\n";

    }
    return 0;
}