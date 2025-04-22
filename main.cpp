#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "BSTh.h"

using namespace std;

// Генерация упорядоченного вектора из 10 уникальных случайных чисел <100
static vector<int> genSet() {
    vector<int> s;
    while (s.size() < 10) {
        int x = rand() % 100;
        if (find(s.begin(), s.end(), x) == s.end())
            s.push_back(x);
    }
    sort(s.begin(), s.end());
    return s;
}

// MERGE: слияние двух упорядоченных последовательностей с сохранением дубликатов
template<typename Seq>
Seq MERGE(const Seq& s1, const Seq& s2) {
    Seq res;
    merge(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(res));
    return res;
}

// EXCL: удаление подпоследовательности из основной
template<typename Seq>
Seq EXCL(const Seq& mainSeq, const Seq& subSeq) {
    Seq res(mainSeq);
    auto it = search(res.begin(), res.end(), subSeq.begin(), subSeq.end());
    if (it != res.end())
        res.erase(it, next(it, subSeq.size()));
    return res;
}

// CHANGE: замена части последовательности на другую с заданной позиции
template<typename Seq>
Seq CHANGE(const Seq& mainSeq, size_t pos, const Seq& subSeq) {
    Seq res(mainSeq);
    if (pos <= res.size()) {
        auto it = next(res.begin(), pos);
        res.erase(it, next(it, min(subSeq.size(), res.size() - pos)));
        res.insert(it, subSeq.begin(), subSeq.end());
    }
    return res;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned>(time(nullptr)));

    // 1. Генерация пяти множеств A, B, C, D, E
    auto A = genSet();
    auto B = genSet();
    auto C = genSet();
    auto D = genSet();
    auto E = genSet();

    Tree tA(A.begin(), A.end()),
         tB(B.begin(), B.end()),
         tC(C.begin(), C.end()),
         tD(D.begin(), D.end()),
         tE(E.begin(), E.end());

    cout << "Исходные множества:\n";
    tA.Display(); tB.Display(); tC.Display(); tD.Display(); tE.Display();

    // 2. T1 = A ∩ B
    Tree T1;
    set_intersection(tA.begin(), tA.end(), tB.begin(), tB.end(),
                     outinserter(T1, myiter(nullptr)));
    cout << "\nT1 = A ∩ B:\n"; T1.Display();

    // 3. T2 = T1 ∪ C
    Tree T2;
    set_union(T1.begin(), T1.end(), tC.begin(), tC.end(),
              outinserter(T2, myiter(nullptr)));
    cout << "\nT2 = T1 ∪ C:\n"; T2.Display();

    // 4. T3 = D ⊕ E
    Tree T3;
    set_symmetric_difference(tD.begin(), tD.end(),
                             tE.begin(), tE.end(),
                             outinserter(T3, myiter(nullptr)));
    cout << "\nT3 = D ⊕ E:\n"; T3.Display();

    // 5. Итог = T2 ∪ T3
    Tree T;
    set_union(T2.begin(), T2.end(), T3.begin(), T3.end(),
              outinserter(T, myiter(nullptr)));
    cout << "\nИтог = T2 ∪ T3:\n"; T.Display();

    // --- Доп. операции над последовательностями ---
    cout << "\nA: "; for (int x : A) cout << x << " "; cout << "\n";
    cout << "B: "; for (int x : B) cout << x << " "; cout << "\n";

    auto merged = MERGE(A, B);
    cout << "MERGE(A,B): ";
    for (int x : merged) cout << x << " "; cout << "\n";
    Tree tM(merged.begin(), merged.end());
    cout << "Дерево из MERGE:\n"; tM.Display();

    vector<int> subExcl(merged.begin() + 2, merged.begin() + 5);
    auto excl = EXCL(merged, subExcl);
    cout << "EXCL (удалили subseq [";
    for (int x : subExcl) cout << x << " ";
    cout << "]): ";
    for (int x : excl) cout << x << " "; cout << "\n";
    Tree tE2(excl.begin(), excl.end());
    cout << "Дерево из EXCL:\n"; tE2.Display();

    vector<int> subChange{100,200,300};
    auto changed = CHANGE(merged, 3, subChange);
    cout << "CHANGE (с позиции 3 -> {100,200,300}): ";
    for (int x : changed) cout << x << " "; cout << "\n";
    Tree tC2(changed.begin(), changed.end());
    cout << "Дерево из CHANGE:\n"; tC2.Display();

    return 0;
}
