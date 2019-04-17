#include <iostream>
#include <cmath>
#include <random>
#include <ctime>

struct Point {
    double x, y, z;
};

struct Node {
    Node *next, *prev;
    Point pt;
};

struct List {
    Node *head;
    int size;
};

double rand_num(double min, double max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_real_distribution<double> dist(min, max);

    return dist(gen);
}

double distance(Point pt) {
    return sqrt((pt.x * pt.x) + (pt.y * pt.y) + (pt.z * pt.z));
}



int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}