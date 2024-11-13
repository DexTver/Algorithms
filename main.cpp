#include "sets/set_array.h"
#include "sets/set_list.h"
#include "sets/set_bool_array.h"
#include "sets/set_machine_word.h"

#include <iostream>
#include <random>
#include <chrono>

using namespace std;

void generatorWithFixedLen(char a[], int len) {
    mt19937 rnd(random_device{}());
    char digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i = 9; i > 0; --i) {
        int j = uniform_int_distribution<int>(0, i)(rnd);
        swap(digits[i], digits[j]);
    }
    for (int i = 0; i < len; ++i) {
        a[i] = digits[i];
    }
}

int main() {
    char a[U + 1]{}, b[U + 1]{}, c[U + 1]{}, d[U + 1]{};
    long long array = 0, list = 0, bool_array = 0, machine_word = 0;
    for (int ln = 1; ln <= 10; ++ln) {
        for (int i = 0; i < 1000; ++i) {
            generatorWithFixedLen(a, ln);
            generatorWithFixedLen(b, ln);
            generatorWithFixedLen(c, ln);
            generatorWithFixedLen(d, ln);

            set_array array_a(a), array_b(b), array_c(c), array_d(d);
            set_list list_a(a), list_b(b), list_c(c), list_d(d);
            set_bool_array bool_array_a(a), bool_array_b(b), bool_array_c(c), bool_array_d(d);
            set_machine_word machine_word_a(a), machine_word_b(b), machine_word_c(c), machine_word_d(d);

            auto start = chrono::high_resolution_clock::now();
            (array_a & array_b) | array_c | array_d;
            auto stop = chrono::high_resolution_clock::now();
            array += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

            start = chrono::high_resolution_clock::now();
            (list_a & list_b) | list_c | list_d;
            stop = chrono::high_resolution_clock::now();
            list += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

            start = chrono::high_resolution_clock::now();
            (bool_array_a & bool_array_b) | bool_array_c | bool_array_d;
            stop = chrono::high_resolution_clock::now();
            bool_array += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();

            start = chrono::high_resolution_clock::now();
            (machine_word_a & machine_word_b) | machine_word_c | machine_word_d;
            stop = chrono::high_resolution_clock::now();
            machine_word += chrono::duration_cast<chrono::nanoseconds>(stop - start).count();
        }
        cout << ln << '\t';
        cout << array / 1000 << '\t';
        cout << list / 1000 << '\t';
        cout << bool_array / 1000 << '\t';
        cout << machine_word / 1000 << endl;
    }
}
