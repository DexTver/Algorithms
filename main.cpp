#include "sets/set_array.h"

#include <iostream>
#include <random>

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
    int ln = 0;

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
        cout << "Enter length of set (1 to 10):\n";
        do {
            cin >> ln;
        } while (ln < 1 || ln > 10);
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

    ((array_a & array_b) | array_c | array_d).show();
}
