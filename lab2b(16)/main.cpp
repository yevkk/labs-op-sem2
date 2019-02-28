#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>

using namespace std;

const int MAX_CAP = 3;

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
        lst.head->msg.time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        lst.tail = lst.head;
        lst.head = lst.head->next;
    } else {
        Node *ptr = new Node;
        if (lst.capacity == 0) lst.head = ptr;
        else lst.tail->next = ptr;
        ptr->msg.text = msg;
        ptr->msg.time = chrono::system_clock::to_time_t(chrono::system_clock::now());
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

void demonstration(List lst){
    string msg;

    cout << "Max capacity:" << MAX_CAP;

    msg = "00000001";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    msg = "00000010";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    cout << "Print (2):\n";
    print(lst, 2);

    msg = "00000011";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    msg = "00000100";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    msg = "00000101";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    msg = "00000110";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";
    msg = "00000111";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    msg = "00001000";
    add_msg(lst, msg);
    cout << "Message \"" << msg << "\" added\n";

    cout << "Print (3):\n";
    print(lst, 3);

    cout << "Print (1):\n";
    print(lst, 1);

    cout << "Print (2):\n";
    print(lst, 2);
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

        if (str == "print_k") {
            do {
                cout << "k (k <= " << MAX_CAP << ") = ";
                cin >> k;
            } while ((k <= 0) || (k > MAX_CAP));
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

        cout << "Enter correct command";
    } while (str != "stop");

    return 0;
}