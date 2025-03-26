#include <iostream>
#include <exception>
#include <string>
#include "screen.h"
#include "shape.h"

class NegativeSizeException : public std::exception {
    std::string msg;

public:
    explicit NegativeSizeException(const std::string &m) : msg(m) {
    }

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

class OutOfScreenException : public std::exception {
    std::string msg;

public:
    explicit OutOfScreenException(const std::string &m) : msg(m) {
    }

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

class diagonal_cross : public shape {
protected:
    point center;
    int size;

public:
    diagonal_cross(point c, int s) : center(c), size(s) {
        if (size <= 0) {
            throw NegativeSizeException("diagonal_cross: size must be positive");
        }
    }

    point north() const override { return point(center.x, center.y + size); }
    point south() const override { return point(center.x, center.y - size); }
    point east() const override { return point(center.x + size, center.y); }
    point west() const override { return point(center.x - size, center.y); }
    point neast() const override { return point(center.x + size, center.y - size); }
    point seast() const override { return point(center.x + size, center.y + size); }
    point nwest() const override { return point(center.x - size, center.y - size); }
    point swest() const override { return point(center.x - size, center.y + size); }

    void draw() override {
        if (!on_screen(nwest().x, nwest().y) ||
            !on_screen(neast().x, neast().y) ||
            !on_screen(seast().x, seast().y) ||
            !on_screen(swest().x, swest().y)) {
            throw OutOfScreenException("diagonal_cross: figure is out of screen bounds");
        }
        put_line(nwest(), seast());
        put_line(neast(), swest());
    }

    void move(int dx, int dy) override {
        center.x += dx;
        center.y += dy;
    }

    void resize(double factor) override {
        if (factor <= 0) {
            throw NegativeSizeException("diagonal_cross: resize factor must be > 0");
        }
        size = static_cast<int>(size * factor);
    }
};

void up(shape &p, const shape &q) {
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

void down(shape &p, const shape &q) {
    point s = q.south();
    point n = p.north();
    p.move(s.x - n.x, s.y - n.y - 1);
}

void left(shape &p, const shape &q) {
    point w = q.west();
    point e = p.east();
    p.move(w.x - e.x - 1, w.y - e.y);
}

void right(shape &p, const shape &q) {
    point e = q.east();
    point w = p.west();
    p.move(e.x - w.x + 1, e.y - w.y);
}

static void safe_shape_refresh() {
    screen_clear();
    // Проходим по всем фигурам
    for (auto it = shape::shapes.begin(); it != shape::shapes.end();) {
        shape *current = *it;
        try {
            current->draw();
            ++it;
        } catch (std::exception &ex) {
            std::cout << "[Draw Error] " << ex.what() << std::endl;
            auto next = it;
            ++next;
            delete current;
            it = next;
        }
        catch (...) {
            std::cout << "[Unknown Draw Error]" << std::endl;
            auto next = it;
            ++next;
            delete current;
            it = next;
        }
    }
    screen_refresh();
}

int main() {
#ifdef LOCAL
    freopen("output.out", "w", stdout);
#endif
    screen_init();

    rectangle hat(point(0, 0), point(14, 5));
    rectangle face(point(16, 0), point(28, 8));
    line brim(point(20, 10), 17);

    diagonal_cross *left_ear = nullptr;
    diagonal_cross *right_ear = nullptr;
    diagonal_cross *tie = nullptr;
    diagonal_cross *bad_cross1 = nullptr;
    diagonal_cross *bad_cross2 = nullptr;
    diagonal_cross *bad_cross3 = nullptr;
    diagonal_cross *bad_cross4 = nullptr;

    try {
        left_ear = new diagonal_cross(point(5, 15), 2);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] left_ear: " << ex.what() << std::endl;
    }
    try {
        right_ear = new diagonal_cross(point(12, 15), 2);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] right_ear: " << ex.what() << std::endl;
    }
    try {
        tie = new diagonal_cross(point(22, 15), 3);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] tie: " << ex.what() << std::endl;
    }
    try {
        bad_cross1 = new diagonal_cross(point(5, 5), -4);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] bad_cross1: " << ex.what() << std::endl;
    }
    try {
        bad_cross2 = new diagonal_cross(point(2, 2), 9999);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] bad_cross2: " << ex.what() << std::endl;
    }
    try {
        bad_cross3 = new diagonal_cross(point(5, 20), 2);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] bad_cross3: " << ex.what() << std::endl;
    }
    try {
        bad_cross4 = new diagonal_cross(point(10, 20), 2);
    } catch (const std::exception &ex) {
        std::cout << "[Creation Error] bad_cross4: " << ex.what() << std::endl;
    }

    safe_shape_refresh();
    std::cout << "=== Generated... ===\n";

    hat.rotate_right();
    brim.resize(2.0);
    face.resize(1.2);

    try {
        if (bad_cross3) {
            bad_cross3->resize(-3.0);
        }
    } catch (const std::exception &ex) {
        std::cout << "[Resize Error] bad_cross3: " << ex.what() << "\n";
    }
    try {
        if (bad_cross3) {
            bad_cross3->resize(100);
        }
    } catch (const std::exception &ex) {
        std::cout << "[Resize Error] bad_cross3: " << ex.what() << "\n";
    }

    safe_shape_refresh();
    std::cout << "=== Prepared... ===\n";

    face.move(-3, 10);
    up(brim, face);
    up(hat, brim);
    if (tie) down(*tie, face);
    if (left_ear) left(*left_ear, face);
    if (right_ear) right(*right_ear, face);
    if (bad_cross4) bad_cross4->move(100, 100);
    safe_shape_refresh();
    std::cout << "=== Ready! ===\n";

    screen_destroy();
    return 0;
}
