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
    do{
        cout << '"' << ptr->msg.text << "\"  " << ctime(&(ptr->msg.time));
        ptr = ptr->next;
    }while (ptr != lst.head);
}

int main() {
    List lst; string str, msg;



    return 0;
}