#include <iostream>
#include <cmath>
#include <random>
#include <ctime>

const int N = 101;
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

void add_element(Point *points, int &size) {
    points[size] = {rand_num(-MAX, MAX), rand_num(-MAX, MAX), rand_num(-MAX, MAX)};
    size++;
}

void swap(Point &pt1, Point &pt2) {
    Point pt = pt2;
    pt2 = pt1;
    pt1 = pt;
}

void print_array(Point *points, int size) {
    std::cout << "Print array:\n";
    for (int i = 0; i < size; i++) {
        std::cout << "x: " << points[i].x << std::endl;
        std::cout << "y: " << points[i].y << std::endl;
        std::cout << "z: " << points[i].z << std::endl;
        std::cout << "dist: " << points[i].dist() << std::endl << std::endl;
    }
}


void bubble_sort(Point *points, int size) {
    bool flag;
    for (int i = 0; i < size - 1; i++) {
        flag = true;
        for (int j = 0; j < size - i - 1; j++)
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
            swap(points[i], points[j]);
            i++;
        }
    }
    swap(points[i], points[last]);
    return i;
}

void quick_sort(Point *points, int first, int size, char pivotMode = 'l') {
    if (first < size - 1) {
        int p = partition(points, first, size - 1, pivotMode);
        quick_sort(points, first, p - 1, pivotMode);
        quick_sort(points, p + 1, size - 1, pivotMode);
    }
}

template<typename T>
void copy_array(T *src, T *dst, int size) {
    for (int i = 0; i < size; i++)
        dst[i] = src[i];
}

void merge_sorted_parts(Point *src, Point *dst, int first, int middle, int size) {
    int i = first, j = middle;

    for (int k = first; k < size; k++) {
        if ((i < middle) && ((j >= size) || (src[i].dist() < src[j].dist()))) {
            dst[k] = src[i];
            i++;
        } else {
            dst[k] = src[j];
            j++;
        }

    }
}

void split_for_sort(Point *src, Point *dst, int first, int size) {
    if (size - first < 2) return;

    int middle = (first + size) / 2;

    split_for_sort(dst, src, first, middle);
    split_for_sort(dst, src, middle, size);

    merge_sorted_parts(src, dst, first, middle, size);
}

void merge_sort(Point *points, int size) { //top-down implementation;
    Point new_array[N];
    copy_array(points, new_array, size);
    split_for_sort(new_array, points, 0, size);
}

int main() {

    return 0;
}