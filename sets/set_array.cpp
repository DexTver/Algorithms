#include <iostream>
#include <cstring>
#include "set_array.h"

set_array::set_array() : n(0), A(new char[U + 1]) {
    A[0] = '\0';
}

set_array::set_array(const char *str) : n(0), A(new char[U + 1]) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (strchr("0123456789", str[i]) && !strchr(A, str[i])) {
            A[n++] = str[i];
        }
    }
    A[n] = '\0';
}

set_array::set_array(const set_array &other) : n(other.n), A(new char[U + 1]) {
    std::copy(other.A, other.A + other.n, A);
    A[n] = '\0';
}

set_array::~set_array() {
    delete[] A;
}

set_array set_array::operator|(const set_array &other) const {
    set_array result(*this);
    for (int i = 0; i < other.n; ++i) {
        if (!strchr(result.A, other.A[i])) {
            result.A[result.n++] = other.A[i];
        }
    }
    result.A[result.n] = '\0';
    return result;
}

set_array set_array::operator&(const set_array &other) const {
    set_array result;
    for (int i = 0; i < n; ++i) {
        if (strchr(other.A, A[i])) {
            result.A[result.n++] = A[i];
        }
    }
    result.A[result.n] = '\0';
    return result;
}

set_array set_array::operator~() const {
    set_array result;
    for (char c = '0'; c <= '9'; ++c) {
        if (!strchr(A, c)) {
            result.A[result.n++] = c;
        }
    }
    result.A[result.n] = '\0';
    return result;
}

set_array &set_array::operator=(const set_array &other) {
    if (this != &other) {
        for (int i = 0; i < other.n; ++i) {
            A[i] = other.A[i];
        }
        n = other.n;
        A[n] = '\0';
    }
    return *this;
}

void set_array::show() const {
    std::cout << "[";
    bool first = true;
    for (int i = 0; i < n; ++i) {
        if (!first) std::cout << ", ";
        std::cout << A[i];
        first = false;
    }
    std::cout << "]";
}

int set_array::power() const {
    return n;
}
