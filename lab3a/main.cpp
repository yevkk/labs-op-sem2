#include <iostream>
#include <cmath>
#include <random>
#include <ctime>

const int N = 100;
const double MAX = 50;

struct Point {
    double x, y, z;

    double dist() {
        return sqrt((x * x) + (y * y) + (z * z));
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

void print_array(Point *points, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << "x: " << points[i].x << std::endl;
        std::cout << "y: " << points[i].y << std::endl;
        std::cout << "z: " << points[i].z << std::endl;
        std::cout << "dist: " << points[i].dist() << std::endl << std::endl;
    }
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

int partition(Point *points, int first, int last, char pivotMode = 'l') {
    double pivot = (pivotMode == 'l') ? points[first].dist() : points[last].dist();
    int i = first;
    if (pivotMode == 'l') swap(points[first], points[last]);

    for (int j = first; j <= last - 1; j++) {
        if (points[j].dist() <= pivot) {
            swap(points[i],
                 points[j]);
            i++;
        }
    }
    swap(points[i], points[last]);
    return i;
}

void quick_sort(Point *points, int first, int last, char pivotMode = 'l') {
    if (first < last) {
        int p = partition(points, first, last, pivotMode);
        quick_sort(points, first, p - 1, pivotMode);
        quick_sort(points, p + 1, last, pivotMode);
    }
}

int main() {
    Point points[N];
    int size = 0;

    for (int i = 0; i < 10; i++)
        add_element(points, size);
    print_array(points, size);
    std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
    quick_sort(points, 0, size - 1);
    print_array(points, size);


    return 0;
}