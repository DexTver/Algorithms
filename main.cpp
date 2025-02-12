#include <iostream>
#include "screen.h"
#include "shape.h"

// Класс для косого креста
class diagonal_cross : public shape, public rotatable {
protected:
    point center;
    int size;

public:
    diagonal_cross(point c, int s) : center(c), size(s) {
    }

    point north() const override { return point(center.x, center.y - size); }
    point south() const override { return point(center.x, center.y + size); }
    point east() const override { return point(center.x + size, center.y); }
    point west() const override { return point(center.x - size, center.y); }
    point neast() const override { return point(center.x + size, center.y - size); }
    point seast() const override { return point(center.x + size, center.y + size); }
    point nwest() const override { return point(center.x - size, center.y - size); }
    point swest() const override { return point(center.x - size, center.y + size); }

    void draw() override {
        put_line(nwest(), seast()); // Рисуем диагональ из левого верхнего в правый нижний угол
        put_line(neast(), swest()); // Рисуем диагональ из правого верхнего в левый нижний угол
    }

    void move(int a, int b) override {
        center.x += a;
        center.y += b;
    }

    void resize(double d) override {
        size *= d;
    }

    // Реализация поворота для косого креста
    void rotate_left() override {
        // Поворот на 90 градусов влево
        int temp = size;
        size = size / 2; // Пример изменения размера при повороте
    }

    void rotate_right() override {
        // Поворот на 90 градусов вправо
        int temp = size;
        size = size * 2; // Пример изменения размера при повороте
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

int main() {
    setlocale(LC_ALL, "Rus");
    screen_init();

    // Создаем фигуры
    rectangle body(point(30, 10), point(50, 20)); // Тело (прямоугольник)
    diagonal_cross tie(point(40, 25), 3); // Галстук (косой крест)
    diagonal_cross left_ear(point(20, 15), 2); // Левое ухо (косой крест)
    diagonal_cross right_ear(point(60, 15), 2); // Правое ухо (косой крест)

    // Выводим исходный набор фигур
    shape_refresh();
    std::cout << "=== Исходный набор фигур ===\n";
    std::cin.get(); // Ждем нажатия Enter

    // Подготовка фигур к сборке
    tie.resize(1.5); // Увеличиваем галстук
    left_ear.rotate_left(); // Поворачиваем левое ухо влево
    right_ear.rotate_right(); // Поворачиваем правое ухо вправо

    // Выводим результат подготовки
    shape_refresh();
    std::cout << "=== Подготовка фигур ===\n";
    std::cin.get(); // Ждем нажатия Enter

    // Сборка изображения
    up(tie, body); // Размещаем галстук над телом
    left(left_ear, body); // Размещаем левое ухо слева от тела
    right(right_ear, body); // Размещаем правое ухо справа от тела

    // Выводим финальное изображение
    shape_refresh();
    std::cout << "=== Финальное изображение ===\n";
    std::cin.get(); // Ждем нажатия Enter

    screen_destroy();
    return 0;
}
