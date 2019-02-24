#include <iostream>
#include <vector>
#include <string>
#include <random>

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

double rand_num(double max){
    static random_device rd;
    static mt19937 mt(rd());
    static uniform_real_distribution<double> dist(0, max);

    return dist(mt);
}

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
    cout << "Element ((" << x << ", " << y << "), " << radius << ") added;\n";
}

void enqueue(Circle *queue_data, int &queue_size, double x, double y, double radius) {
    if (queue_size + 1 > MAX_SIZE) {
        cout << "Element cannot be added, queue is full\n";
        return;
    }
    queue_data[queue_size].center.x = x;
    queue_data[queue_size].center.y = y;
    queue_data[queue_size].radius = radius;
    queue_size++;
    cout << "Element ((" << x << ", " << y << "), " << radius << ") added;\n";
}

void enqueue(vector<Circle> &queue, double x, double y, double radius) {
    Circle circle = {x, y, radius};
    queue.push_back(circle);
    cout << "Element ((" << x << ", " << y << "), " << radius << ") added;\n";
}

void dequeue(List &queue) {
    if (is_queue_empty(queue)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    Node *ptr = queue.head->next;
    cout << "Element ((" << queue.head->circle.center.x << ", " << queue.head->circle.center.y << "), " << queue.head->circle.radius << ")";
    delete queue.head;
    queue.head = ptr;
    cout  << "removed;\n";
}

void dequeue(Circle *queue_data, int &queue_size) {
    if (is_queue_empty(queue_size)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    cout << "Element ((" << queue_data[0].center.x << ", " << queue_data[0].center.y << "), " << queue_data[0].radius << ") ";
    for (int i = 0; i < queue_size - 1; i++) {
        queue_data[i] = queue_data[i + 1];
    }
    queue_size--;
    cout  << "removed;\n";
}

void dequeue(vector<Circle> &queue) {
    if (is_queue_empty(queue)) {
        cout << "Element cannot be removed, queue is empty\n";
        return;
    }
    cout << "Element ((" << queue.at(0).center.x << ", " << queue.at(0).center.y << "), " << queue.at(0).radius << ") ";
    queue.erase(queue.begin());
    cout  << "removed;\n";
}

void input_values(double &x, double &y, double &radius) {
    cout << "Input:\n" << "  center x:";
    cin >> x;
    cout << "  center y:";
    cin >> y;
    cout << "  radius:";
    cin >> radius;
}

void print_queue(List queue){
    if (is_queue_empty(queue)) cout << "Queue is empty;\n";
    else {
        Node* ptr = queue.head;
        int i = 0;
        while(ptr!=nullptr){
            cout << ++i << ": ((" << ptr->circle.center.x << ", " << ptr->circle.center.y << "), " << ptr->circle.radius << ");\n";
            ptr = ptr->next;
        }
    }
}

void print_queue(Circle *queue_data, int &queue_size){
    if (is_queue_empty(queue_size)) cout << "Queue is empty;\n";
    else {
        for (int i = 0; i < queue_size; i++) {
            cout << i+1 << ": ((" << queue_data[i].center.x << ", " << queue_data[i].center.y << "), " << queue_data[i].radius << ");\n";
        }
    }
}

void print_queue(vector<Circle> queue){
    if (is_queue_empty(queue)) cout << "Queue is empty;\n";
    else {
        for (int i = 0; i < queue.size(); i++) {
            cout << i+1 << ": ((" << queue[i].center.x << ", " << queue[i].center.y << "), " << queue[i].radius << ");\n";
        }
    }
}

void demonstrate(List queue){
    create_empty(queue);
    int num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        enqueue(queue, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue);
    num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        dequeue(queue);
    }
    cout << "Print:\n";
    print_queue(queue);
    num = (int)rand_num(3);
    for(int i = 0; i< num; i++){
        enqueue(queue, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue);
}

void demonstrate(Circle *queue_data, int &queue_size){
    create_empty(queue_size);
    int num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        enqueue(queue_data, queue_size, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue_data, queue_size);
    num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        dequeue(queue_data, queue_size);
    }
    cout << "Print:\n";
    print_queue(queue_data, queue_size);
    num = (int)rand_num(3);
    for(int i = 0; i< num; i++){
        enqueue(queue_data, queue_size, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue_data, queue_size);
}

void demonstrate(vector<Circle> queue){
    create_empty(queue);
    int num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        enqueue(queue, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue);
    num = (int)rand_num(5);
    for(int i = 0; i< num; i++){
        dequeue(queue);
    }
    cout << "Print:\n";
    print_queue(queue);
    num = (int)rand_num(3);
    for(int i = 0; i< num; i++){
        enqueue(queue, rand_num(10), rand_num(10),rand_num(10));
    }
    cout << "Print:\n";
    print_queue(queue);
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
        cout << "\nEnter command: ";
        cin >> str;

        if (str == "stop") break;

        if (str == "select_mode") {
            cout << "1 - list\n2 - array\n3 - vector\n";
            do {
                cin >> mode;

                if ((mode < 1) || (mode > 3)) cout << "select correct mode\n";
                else cout<< "Mode " << mode << " selected\n";
            } while ((mode < 1) || (mode > 3));
            continue;
        }

        if (str == "create") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
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
                default: break;
            }
            continue;
        }

        if (str == "add") {
            input_values(x, y, r);
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
                    break;
                }
                case 1: {
                    enqueue(queue_list, x, y, r);
                    break;
                }
                case 2: {
                    enqueue(queue_array.data, queue_array.size, x, y, r);
                    break;
                }
                case 3: {
                    enqueue(queue_vector, x, y, r);
                    break;
                }
                default: break;
            }
            continue;
        }

        if (str == "remove") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
                    break;
                }
                case 1: {
                    dequeue(queue_list);
                    break;
                }
                case 2: {
                    dequeue(queue_array.data, queue_array.size);
                    break;
                }
                case 3: {
                    dequeue(queue_vector);
                    break;
                }
                default: break;
            }
            continue;
        }

        if (str == "is_empty") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
                    break;
                }
                case 1: {
                    cout << boolalpha << is_queue_empty(queue_list) << endl;
                    break;
                }
                case 2: {
                    cout << boolalpha << is_queue_empty(queue_array.size) << endl;
                    break;
                }
                case 3: {
                    cout << boolalpha << is_queue_empty(queue_vector) << endl;
                    break;
                }
                default: break;
            }
            continue;
        }

        if (str == "print") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
                    break;
                }
                case 1: {
                    print_queue(queue_list);
                    break;
                }
                case 2: {
                    print_queue(queue_array.data, queue_array.size);
                    break;
                }
                case 3: {
                print_queue(queue_vector);
                break;
                }
                default: break;
            }
            continue;
        }

        if (str == "demo") {
            switch (mode){
                case 0: {
                    cout << "Select mode first;\n";
                    break;
                }
                case 1: {
                    demonstrate(queue_list);
                    break;
                }
                case 2: {
                    demonstrate(queue_array.data, queue_array.size);
                    break;
                }
                case 3: {
                    demonstrate(queue_vector);
                    break;
                }
                default: break;
            }
            continue;
        }

        if (str == "help"){
            cout << "  -stop\n"
                    "  -demo\n"
                    "  -@benchmark\n"
                    "  -select_mode\n"
                    "  -create\n"
                    "  -add\n"
                    "  -remove\n"
                    "  -is_empty\n"
                    "  -print\n";
            continue;
        }

        cout << "Enter correct command (help)\n";

    }
    return 0;
}