#include <iostream>
#include <exception>
#include <string>
#include "screen.h"
#include "shape.h"

class NegativeSizeException : public std::exception {
    std::string msg;
public:
    explicit NegativeSizeException(const std::string &m) : msg(m) {}

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

class OutOfScreenException : public std::exception {
    std::string msg;
public:
    explicit OutOfScreenException(const std::string &m) : msg(m) {}

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


int main() {
    setlocale(LC_ALL, "C");
    screen_init();


    try {
        diagonal_cross bad_cross(point(5, 5), -4);
    }
    catch (const NegativeSizeException &ex) {
        std::cout << "[Creation Error] " << ex.what() << std::endl;
    }
    catch (...) {
        std::cout << "[Unknown error while creating bad_cross]\n";
    }


    try {
        rectangle hat(point(0, 0), point(14, 5));
        rectangle face(point(16, 0), point(28, 8));
        line brim(point(20, 10), 17);

        diagonal_cross left_ear(point(5, 15), 2);
        diagonal_cross right_ear(point(12, 15), 2);
        diagonal_cross tie(point(22, 15), 2);

        shape_refresh();
        std::cout << "=== Initial set generated ===\n";
        // std::cin.get();

        hat.rotate_right();
        brim.resize(2.0);
        face.resize(1.2);
        tie.resize(1.5);
        shape_refresh();
        std::cout << "=== Transformed shapes ===\n";
        // std::cin.get();

        face.move(-3, 10);
        up(brim, face);
        up(hat, brim);
        down(tie, face);
        left(left_ear, face);
        right(right_ear, face);

        shape_refresh();
        std::cout << "=== Final composition ===\n";
        // std::cin.get();
    }
    catch (const OutOfScreenException &ex) {
        std::cout << "[Drawing Error] " << ex.what() << std::endl;
        std::cout << "Try reducing size or changing position of the shape.\n";
    }
    catch (const NegativeSizeException &ex) {
        std::cout << "[Size Error during execution] " << ex.what() << std::endl;
    }
    catch (...) {
        std::cout << "[Unknown error during shape composition]\n";
    }

    screen_destroy();
    return 0;
}
