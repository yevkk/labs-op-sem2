#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>
using namespace std;

const int MAX = 10;

struct Message {
    string text;
    time_t time;
};

struct Node {
    Message msg;
    Node* next;
};

struct List {
    Node* head;
    Node* tail;
    int capacity;
};

int main() {
    List lst;


    //time_t f = chrono::system_clock::to_time_t(chrono::system_clock::now());
    return 0;
}