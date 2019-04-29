#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <chrono>
#include <functional>

const int NUM = 100;   //max number of elements in array;
const int NUM1 = 1000; //number of elements in array for benchmark;
const int MAX = 100;   //max value;

//---------- TOOLS ----------

int rand_num(int min, int max) {
    static std::random_device rd;
    static std::seed_seq seed{rd(), static_cast<unsigned int>(time(nullptr))};
    static std::mt19937_64 gen(seed);
    static std::uniform_int_distribution<int> dist(min, max);

    return dist(gen);
}

void set_element(int *arr, int pos) {
    arr[pos] = rand_num(0, MAX);
}

void print_array(int *arr, int &size) {
    std::cout << "Array:\n";
    for (int i = 0; i < size; i++)
        std::cout << arr[i] << " ";
    std::cout << std::endl;
}
//---------- BINARY TREE SORT ----------

//---------- BUCKET SORT ----------
//---------- BENCHMARK ----------

template<typename Func>
auto time_ms(const Func &func) {
    using namespace std::chrono;
    const auto begin = system_clock::now();

    func();

    const auto end = system_clock::now();

    return duration_cast<milliseconds>(end - begin).count();
}

void benchmark(int size) {
    int arr[NUM1], *pt;
    int time;

    std::cout << "\nBenchmark (" << size << " elements):\n";

//    std::cout << "-@@@ Sort: ";
//    for (int i = 0; i < size; i++) {
//        set_element(points, i);
//    }
//    pt = &points[0];
//    time = time_ms([&pt, &size]() { @@@_sort(pt, size); });
//    std::cout << time << " ms;\n";
//
}


int main() {
    int arr[NUM];
    int size = 5, command;

    for (int i = 0; i < size; i++) {
        set_element(arr, size);
    }

    std::cout << "Array size is set to " << size << std::endl;
    std::cout << "Command list:\n";
    std::cout << "\t1 - Set array size\n";
    std::cout << "\t2 - Demo binary tree sort\n";
    std::cout << "\t3 - Demo bucket sort\n";
    std::cout << "\t4 - Benchmark\n";
    std::cout << "\t5 - Stop\n";

    while (true) {
        std::cout << "\nEnter command number: ";
        std::cin >> command;

        if (command == 6) break;

        switch (command) {
            case 1: {
                size = -1;
                while ((size <= 0) || (size > NUM)) {
                    std::cout << "Enter new size (1-" << NUM << "): ";
                    std::cin >> size;
                }
                for (int i = 0; i < size; i++) {
                    set_element(arr, i);
                }
                std::cout << "Array size is set to " << size << ";\n";
                break;
            }
            case 2: {
                print_array(arr, size);
                bt_sort(arr, size);
                break;
            }
            case 3: {
                print_array(arr, size);
                break;
            }
            case 4: {
                print_array(arr, size);
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