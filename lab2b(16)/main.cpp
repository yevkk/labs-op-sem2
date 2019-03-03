#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
#include <random>

using std::cout;
using std::cin;
using std::string;

const int MAX_CAP = 10;

struct Message {
    string text;
    time_t time;
};

struct Node {
    Message msg;
    Node *next;
};

struct List {
    Node *head = nullptr;
    Node *tail = nullptr;
    int capacity = 0;
};

void add_msg(List &lst, string &msg) {
    if (lst.capacity == MAX_CAP) {
        lst.head->msg.text = msg;
        lst.head->msg.time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        lst.tail = lst.head;
        lst.head = lst.head->next;
    } else {
        Node *ptr = new Node;
        if (lst.capacity == 0) lst.head = ptr;
        else lst.tail->next = ptr;
        ptr->msg.text = msg;
        ptr->msg.time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        ptr->next = lst.head;
        lst.tail = ptr;
        lst.capacity++;
    }
}

void print(List &lst, int k) {
    Node *ptr = lst.head;
    for (int i = 0; i < (lst.capacity - k); i++) ptr = ptr->next;
    do {
        cout << '"' << ptr->msg.text << "\"  " << ctime(&(ptr->msg.time));
        ptr = ptr->next;
    } while (ptr != lst.head);
}

double rand_num(double max) {
    static std::random_device rd;
    static std::seed_seq seed { rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_real_distribution<double> dist(0, max);

    return dist(gen);
}

void demonstration(List lst){
    string msg;
    int num = 0;

    cout << "Max capacity:" << MAX_CAP << std::endl;

    int count = 1 + (int) rand_num(MAX_CAP - 3);
    for (int i = 0; i < count; i++){
        msg = "Message " + std::to_string(num);
        add_msg(lst, msg);
        cout << msg << " added\n";
        num++;
    }

    count = 1 + (int) rand_num(lst.capacity);
    cout << "\nPrint (" << count << ")\n";
    print(lst, count);

    count = 1 + (int) rand_num(lst.capacity);
    cout << "\nPrint (" << count << ")\n";
    print(lst, count);

    cout << "\nPrint all:\n";
    print(lst, lst.capacity);

    count = 1 + (int) rand_num(MAX_CAP-2);
    for (int i = 0; i < count; i++){
        msg = "Message " + std::to_string(num);
        add_msg(lst, msg);
        cout << msg << " added\n";
        num++;
    }

    cout << "\nPrint all:\n";
    print(lst, lst.capacity);

    count = 1 + (int) rand_num(lst.capacity);
    cout << "\nPrint (" << count << ")\n";
    print(lst, count);
}

int main() {
    List lst;
    string str, msg;
    int k;

    do {
        cout << "\nEnter commmand: ";
        cin >> str;

        if (str == "add") {
            cout << "Message text: ";
            cin >> msg;
            add_msg(lst, msg);
            continue;
        }

        if (str == "print") {
            do {
                cout << "k (0 < k <= " << lst.capacity << ") = ";
                cin >> k;
            } while ((k <= 0) || (k > lst.capacity));
            print(lst, k);
            continue;
        }

        if (str == "demo") {
            demonstration(lst);
            continue;
        }

        if (str == "help") {
            cout << "-stop\n" << "-add\n" << "-print_k\n" << "-demo\n";
            continue;
        }

        if (str!= "stop") cout << "Enter correct command";
    } while (str != "stop");

    return 0;
}