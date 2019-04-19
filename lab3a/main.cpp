#include <iostream>
#include <cmath>
#include <random>
#include <ctime>

const int N = 100;
const double MAX = 50;

struct Point {
    double x, y, z;
    double dist() {
        return sqrt((x*x) + (y*y) + (z*z));
    }
};

double rand_num(double min, double max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_real_distribution<double> dist(min, max);

    return dist(gen);
}

void add_element(Point *points, int &n) {
    points[n] = {rand_num(-MAX, MAX), rand_num(-MAX, MAX), rand_num(-MAX, MAX)};
    n++;
}

void swap(Point &pt1, Point &pt2) {
    Point pt = pt2;
    pt2 = pt1;
    pt1 = pt;
}

void bubble_sort(Point *points, int n) {
    bool flag;
    for (int i = 0; i < n - 1; i++) {
        flag = true;
        for (int j = 0; j < n - i - 1; j++)
            if (points[j].dist() > points[j + 1].dist()) {
                flag = false;
                swap(points[j], points[j + 1]);
            }
        if (flag) return;
    }
}

void print_array(Point *points, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << "x: " << points[i].x << std::endl;
        std::cout << "y: " << points[i].y << std::endl;
        std::cout << "z: " << points[i].z << std::endl;
        std::cout << "dist: " << points[i].dist() << std::endl << std::endl;
    }
}


int main() {
    Point points[N];
    int size = 0, x=5;

    for (int i = 0; i < 10; i++)
        add_element(points, size);
    print_array(points, size);
    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
    bubble_sort(&points[x], size-x);
    print_array(points, size);


    return 0;
}