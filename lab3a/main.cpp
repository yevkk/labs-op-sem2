#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <functional>

const int NUM = 100;
const long NUM1 = 40000;
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

void add_element(Point *points, int size) {
    points[size] = {rand_num(-MAX, MAX), rand_num(-MAX, MAX), rand_num(-MAX, MAX)};
}

void swap(Point &pt1, Point &pt2) {
    Point pt = pt2;
    pt2 = pt1;
    pt1 = pt;
}

void print_array(Point *points, int &size, bool distOnly = false) {
    std::cout << "Elements:\n";
    for (int i = 0; i < size; i++) {
        std::cout << i + 1 << ") ";
        if (!distOnly) {
            std::cout << std::endl;
            std::cout << "x: " << points[i].x << std::endl;
            std::cout << "y: " << points[i].y << std::endl;
            std::cout << "z: " << points[i].z << std::endl;
        }
        std::cout << "dist: " << points[i].dist() << std::endl;
    }
    std::cout << std::endl;
}


void bubble_sort(Point *points, int &size, bool print = false) {
    bool flag;
    for (int i = 0; i < size - 1; i++) {
        flag = true;
        for (int j = 0; j < size - i - 1; j++)
            if (points[j].dist() > points[j + 1].dist()) {
                flag = false;
                swap(points[j], points[j + 1]);
                if (print) print_array(points, size, true);
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

void quick_sort(Point *points, int first, int size, bool print = false,
                char pivotMode = 'l') {  //leftmost pivot by default, set pivotMode = "r" to use rightmost pivot;
    if (first < size - 1) {
        int p = partition(points, first, size - 1, pivotMode);
        quick_sort(points, first, p - 1, print, pivotMode);
        if (print) print_array(points, size, true);
        quick_sort(points, p + 1, size - 1, print, pivotMode);
        if (print) print_array(points, size, true);
    }
}

template<typename T>
void copy_array(T *src, T *dst, int &size) {
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

void split_for_sort(Point *src, Point *dst, int first, int size, bool print) {
    if (size - first < 2) return;

    int middle = (first + size) / 2;

    split_for_sort(dst, src, first, middle, print);
    split_for_sort(dst, src, middle, size, print);

    merge_sorted_parts(src, dst, first, middle, size);
    if (print) print_array(dst, size, true);
}

void merge_sort(Point *points, int &size, bool print = false) { //top-down implementation;
    Point new_array[NUM1];
    copy_array(points, new_array, size);
    split_for_sort(new_array, points, 0, size, print);
}

template<typename Func>
auto time_ms(const Func &func) {
    using namespace std::chrono;
    const auto begin = system_clock::now();

    func();

    const auto end = system_clock::now();

    return duration_cast<milliseconds>(end - begin).count();
}

void benchmark(int size) {
    Point points[NUM1], *pt;
    int time;

    std::cout << "\nBenchmark (" << size << " elements):\n";

    std::cout << "-Bubble Sort: ";
    for (int i = 0; i < size; i++) {
        add_element(points, i);
    }
    pt = &points[0];
    time = time_ms([&pt, &size]() { bubble_sort(pt, size); });
    std::cout << time << " ms;\n";

    std::cout << "-Quick Sort: ";
    for (int i = 0; i < size; i++) {
        add_element(points, i);
    }
    pt = &points[0];
    time = time_ms([&pt, &size]() { quick_sort(pt, 0, size); });
    std::cout << time << " ms;\n";

    std::cout << "-Merge Sort: ";
    for (int i = 0; i < size; i++) {
        add_element(points, i);
    }
    pt = &points[0];
    time = time_ms([&pt, &size]() { merge_sort(pt, size); });
    std::cout << time << " ms;\n";
}

int main() {
    Point points[NUM];
    int size = 5, command;

    for (int i = 0; i < size; i++) {
        add_element(points, size);
    }

    std::cout << "Array size is set to " << size << std::endl;
    std::cout << "Command list:\n";
    std::cout << "\t1 - Set array size\n";
    std::cout << "\t2 - Demo bubble sort\n";
    std::cout << "\t3 - Demo quick sort\n";
    std::cout << "\t4 - Demo merge sort\n";
    std::cout << "\t5 - Benchmark\n";
    std::cout << "\t6 - Stop\n";

    while (true) {
        std::cout << "\nEnter command number: ";
        std::cin >> command;

        if (command == 6) break;

        switch (command) {
            case 1: {
                size = -1;
                while ((size < 0) || (size > 100)) {
                    std::cout << "Enter new size (1-" << NUM << "): ";
                    std::cin >> size;
                }
                for (int i = 0; i < size; i++) {
                    add_element(points, i);
                }
                std::cout << "Array size is set to " << size << ";\n";
                break;
            }
            case 2: {
                print_array(points, size);
                bubble_sort(points, size, true);
                break;
            }
            case 3: {
                print_array(points, size);
                quick_sort(points, 0, size, true);
                break;
            }
            case 4: {
                print_array(points, size);
                merge_sort(points, size, true);
                break;
            }
            case 5: {
                benchmark(NUM1);
                break;
            }
            default: {
                break;
            }
        }
    }

    return 0;
}