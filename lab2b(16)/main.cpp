#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
using namespace std;

const int MAX_CAP = 10;

struct Message {
    string text;
    time_t time;
};

struct Node {
    Message msg;
    Node* next;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
    int capacity = 0;
};

void add_msg(List &lst, string &msg){
    if (lst.capacity == MAX_CAP){
        lst.head->msg.text = msg;
        lst.head->msg.time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        lst.tail = lst.head;
        lst.head = lst.head->next;
    } else {
        Node *ptr = new Node;
        if (lst.capacity == 0) lst.head = ptr;
        ptr->msg.text = msg;
        ptr->msg.time = chrono::system_clock::to_time_t(chrono::system_clock::now());
        ptr->next = lst.head;
        lst.tail->next = ptr;
        lst.tail = ptr;
        lst.capacity++;
    }
}



int main() {
    List lst;


    //time_t f = chrono::system_clock::to_time_t(chrono::system_clock::now());
    return 0;
}