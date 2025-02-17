#include <iostream>
#include "screen.h"
#include "shape.h"

class diagonal_cross : public shape {
protected:
    point center;
    int size;

public:
    diagonal_cross(point c, int s) : center(c), size(s) {
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
        put_line(nwest(), seast());
        put_line(neast(), swest());
    }

    void move(int a, int b) override {
        center.x += a;
        center.y += b;
    }

    void resize(double d) override {
        size *= d;
    }
};

// Функция для размещения фигуры слева
void left(shape &p, const shape &q) {
    point w = q.west();
    point e = p.east();
    p.move(w.x - e.x - 1, w.y - e.y);
}

// Функция для размещения фигуры справа
void right(shape &p, const shape &q) {
    point e = q.east();
    point w = p.west();
    p.move(e.x - w.x + 1, e.y - w.y);
}

// Функция для размещения фигуры сверху
void up(shape &p, const shape &q) {
    point n = q.north();
    point s = p.south();
    p.move(n.x - s.x, n.y - s.y + 1);
}

// Функция для размещения фигуры снизу
void down(shape &p, const shape &q) {
    point s = q.south();
    point n = p.north();
    p.move(s.x - n.x, s.y - n.y - 1);
}

int main() {
#ifdef LOCAL
    freopen("output.out", "w", stdout);
#endif
    setlocale(LC_ALL, "Rus");
    screen_init();

    rectangle hat(point(0, 0), point(14, 5));
    rectangle face(point(16, 0), point(28, 8));
    line brim(point(20, 10), 17);
    diagonal_cross left_ear(point(5, 15), 2);
    diagonal_cross right_ear(point(12, 15), 2);
    diagonal_cross tie(point(22, 15), 3);
    shape_refresh();
    std::cout << "=== Generated... ===\n";

    hat.rotate_right();
    brim.resize(2.0);
    face.resize(1.2);
    shape_refresh();
    std::cout << "=== Prepared... ===\n";

    face.move(-3, 10);
    up(brim, face);
    up(hat, brim);
    down(tie, face);
    left(left_ear, face);
    right(right_ear, face);
    shape_refresh();
    std::cout << "=== Ready! ===\n";
    return 0;
}
