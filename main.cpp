#include "sets/set_array.h"
#include "sets/set_list.h"
#include "sets/set_bool_array.h"
#include "sets/set_machine_word.h"

#include <iostream>
#include <random>
#include <chrono>

using namespace std;

void scan_set(char t[U + 1]) {
    char x;
    int cnt = 0;
    do {
        x = (char) getc(stdin);
        if ('0' <= x && x <= '9') {
            bool found = false;
            for (int i = 0; i < cnt; ++i) {
                if (t[i] == x) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                t[cnt++] = x;
            }
        }
    } while (x != '\n');
}

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
    char mode;
    int ln;

    do {
        cout << "Select a mode (Manual 'm' or automatic 'a'):" << endl;
        cin >> mode;
        getchar();
    } while (mode != 'm' && mode != 'a');

    if (mode == 'm') {
        cout << "Enter the sets as a string consisting of numbers.\n";
        cout << "A:";
        scan_set(a);
        cout << "B:";
        scan_set(b);
        cout << "C:";
        scan_set(c);
        cout << "D:";
        scan_set(d);
    } else {
        cout << "Enter length of set (0 to 10):\n";
        cin >> ln;
        generatorWithFixedLen(a, ln);
        generatorWithFixedLen(b, ln);
        generatorWithFixedLen(c, ln);
        generatorWithFixedLen(d, ln);
    }

    if (mode == 'a') {
        cout << "A: " << a << "\n";
        cout << "B: " << b << "\n";
        cout << "C: " << c << "\n";
        cout << "D: " << d << "\n";
    }

    set_array array_a(a), array_b(b), array_c(c), array_d(d);
    set_list list_a(a), list_b(b), list_c(c), list_d(d);
    set_bool_array bool_array_a(a), bool_array_b(b), bool_array_c(c), bool_array_d(d);
    set_machine_word machine_word_a(a), machine_word_b(b), machine_word_c(c), machine_word_d(d);

    auto start = chrono::high_resolution_clock::now();
    (array_a & array_b) | array_c | array_d;
    auto stop = chrono::high_resolution_clock::now();
    cout << "\nArray result:\t\t\t";
    ((array_a & array_b) | array_c | array_d).show();
    cout << " in " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds\n";
    ~array_a;
    ~array_b;
    ~array_c;
    ~array_d;

    start = chrono::high_resolution_clock::now();
    (list_a & list_b) | list_c | list_d;
    stop = chrono::high_resolution_clock::now();
    cout << "List result:\t\t\t";
    ((list_a & list_b) | list_c | list_d).show();
    cout << " in " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds\n";
    ~list_a;
    ~list_b;
    ~list_c;
    ~list_d;

    start = chrono::high_resolution_clock::now();
    (bool_array_a & bool_array_b) | bool_array_c | bool_array_d;
    stop = chrono::high_resolution_clock::now();
    cout << "Bool array result:\t\t";
    ((bool_array_a & bool_array_b) | bool_array_c | bool_array_d).show();
    cout << " in " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds\n";
    ~bool_array_a;
    ~bool_array_b;
    ~bool_array_c;
    ~bool_array_d;

    start = chrono::high_resolution_clock::now();
    (machine_word_a & machine_word_b) | machine_word_c | machine_word_d;
    stop = chrono::high_resolution_clock::now();
    cout << "Machine word result:\t";
    ((machine_word_a & machine_word_b) | machine_word_c | machine_word_d).show();
    cout << " in " << chrono::duration_cast<chrono::nanoseconds>(stop - start).count() << " nanoseconds\n";
    ~machine_word_a;
    ~machine_word_b;
    ~machine_word_c;
    ~machine_word_d;
    cout.flush();

}
