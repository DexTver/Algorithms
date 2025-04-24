//  ╔══════════════════════════════════════════════════════════════════════════╗
//  ║  DDP – Динамическая Двойная   (Дерево + Последовательность) структура  ║
//  ║  Реализация на C++20                                                   ║
//  ║  ▪ Дерево: AVL‑balanced, уникальные ключи                              ║
//  ║  ▪ Последовательность: vector<Node*> с сохранением дубликатов          ║
//  ║  ▪ Поддерживаем мощность (|S|) – число уникальных ключей               ║
//  ║  ▪ Операции                                                            ║
//  ║      •   ∪ , ∩ , △  (sym. diff) над множеством (по дереву)            ║
//  ║          – после выполнения перестраивается sequence в произвольном   ║
//  ║            (случайном) порядке                                         ║
//  ║      •   MERGE  – слияние двух упорядоченных последовательностей       ║
//  ║      •   EXCL   – исключение подпоследовательности                     ║
//  ║      •   CHANGE – замена фрагмента                                     ║
//  ║  ▪ Вывод                                                               ║
//  ║      •   printTree()  – ASCII формат как на скриншоте                  ║
//  ║      •   printSeq()   – строка со значениями                           ║
//  ║  ▪ Генератор randomObject(10,15)                                       ║
//  ╚══════════════════════════════════════════════════════════════════════════╝

#include <bits/stdc++.h>

class DDP {
    struct Node {
        int key;
        Node *l, *r;
        int h;

        explicit Node(int k) : key(k), l(nullptr), r(nullptr), h(1) {
        }
    };

    Node *root = nullptr; // дерево‑множество
    std::vector<Node *> seq; // последовательность (с дубликатами)

    /*────────────────────────────┐
      │   ‑‑‑‑‑  AVL helpers  ‑‑‑ │
      └──────────────────────────*/
    static int height(Node *n) { return n ? n->h : 0; }

    static void upd(Node *n) {
        if (!n) return;
        n->h = std::max(height(n->l), height(n->r)) + 1;
    }

    static Node *rotateRight(Node *y) {
        Node *x = y->l;
        Node *T2 = x->r;
        x->r = y;
        y->l = T2;
        upd(y);
        upd(x);
        return x;
    }

    static Node *rotateLeft(Node *x) {
        Node *y = x->r;
        Node *T2 = y->l;
        y->l = x;
        x->r = T2;
        upd(x);
        upd(y);
        return y;
    }

    static int balance(Node *n) { return n ? height(n->l) - height(n->r) : 0; }

    static Node *insert(Node *n, int key, Node **outPtr) {
        if (!n) {
            *outPtr = new Node(key);
            return *outPtr;
        }
        if (key < n->key)
            n->l = insert(n->l, key, outPtr);
        else if (key > n->key)
            n->r = insert(n->r, key, outPtr);
        else {
            *outPtr = n; // уже есть
            return n;
        }
        upd(n);
        int bf = balance(n);
        if (bf > 1 && key < n->l->key)
            return rotateRight(n);
        if (bf < -1 && key > n->r->key)
            return rotateLeft(n);
        if (bf > 1 && key > n->l->key) {
            n->l = rotateLeft(n->l);
            return rotateRight(n);
        }
        if (bf < -1 && key < n->r->key) {
            n->r = rotateRight(n->r);
            return rotateLeft(n);
        }
        return n;
    }

    static Node *minValue(Node *n) {
        while (n->l) n = n->l;
        return n;
    }

    static Node *erase(Node *n, int key) {
        if (!n) return nullptr;
        if (key < n->key) n->l = erase(n->l, key);
        else if (key > n->key) n->r = erase(n->r, key);
        else {
            if (!n->l || !n->r) {
                Node *tmp = n->l ? n->l : n->r;
                delete n;
                return tmp;
            }
            Node *succ = minValue(n->r);
            n->key = succ->key;
            n->r = erase(n->r, succ->key);
        }
        upd(n);
        int bf = balance(n);
        if (bf > 1 && balance(n->l) >= 0) return rotateRight(n);
        if (bf > 1 && balance(n->l) < 0) {
            n->l = rotateLeft(n->l);
            return rotateRight(n);
        }
        if (bf < -1 && balance(n->r) <= 0) return rotateLeft(n);
        if (bf < -1 && balance(n->r) > 0) {
            n->r = rotateRight(n->r);
            return rotateLeft(n);
        }
        return n;
    }

    /*────────────────────────────┐
      │   util: inorder keys      │
      └──────────────────────────*/
    static void inorder(Node *n, std::vector<int> &v) {
        if (!n) return;
        inorder(n->l, v);
        v.push_back(n->key);
        inorder(n->r, v);
    }

    /*────────────────────────────┐
      │   rebuild helpers         │
      └──────────────────────────*/
    void rebuildTreeFromSeq() {
        root = nullptr;
        for (Node *p: seq) // пойдём по последовательности, но мы вставляем по ключу
        {
            Node *ref = nullptr;
            root = insert(root, p->key, &ref);
            *p = *ref; // synchronize pointer (ref == p if ключ существовал)
        }
    }

    void rebuildSeqRandom() {
        // собрать уникальные ключи и затем тасовать
        std::vector<int> keys;
        inorder(root, keys);
        std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
        seq.clear();
        seq.reserve(keys.size());
        for (int k: keys) {
            Node *ref = nullptr; // получаем указатель уже существующий
            insert(root, k, &ref);
            seq.push_back(ref);
        }
    }

public:
    /*────────────────────────────┐
      │  конструкторы             │
      └──────────────────────────*/
    DDP() = default;

    explicit DDP(const std::vector<int> &initial) {
        for (int k: initial) add(k);
    }

    /*────────────────────────────┐
      │  базовые операции         │
      └──────────────────────────*/
    void add(int key) {
        Node *ref = nullptr;
        root = insert(root, key, &ref);
        seq.push_back(ref);
    }

    void removeOne(int key) {
        // удаляет одно появление из последовательности и возможно из дерева
        auto it = std::find_if(seq.begin(), seq.end(), [&](Node *n) { return n->key == key; });
        if (it == seq.end()) return;
        seq.erase(it);
        if (std::none_of(seq.begin(), seq.end(), [&](Node *n) { return n->key == key; }))
            root = erase(root, key);
    }

    size_t cardinality() const { return sizeUnique(); }

    size_t sizeUnique() const {
        std::vector<int> v;
        inorder(root, v);
        return v.size();
    }

    /*────────────────────────────┐
      │  SET OPERATIONS           │
      └──────────────────────────*/
    void unionWith(const DDP &other) {
        std::vector<int> keys;
        inorder(other.root, keys);
        for (int k: keys) {
            Node *ref = nullptr;
            root = insert(root, k, &ref);
        }
        rebuildSeqRandom();
    }

    void intersectWith(const DDP &other) {
        std::vector<int> keys;
        inorder(root, keys); // мои ключи
        for (int k: keys) {
            if (!other.contains(k)) {
                root = erase(root, k);
            }
        }
        rebuildSeqRandom();
    }

    void symDiffWith(const DDP &other) {
        std::vector<int> keysA;
        inorder(root, keysA);
        std::vector<int> keysB;
        inorder(other.root, keysB);
        std::unordered_set<int> setB(keysB.begin(), keysB.end());
        // remove common
        for (int k: keysA) {
            if (setB.count(k)) root = erase(root, k);
        }
        // add those in B not in A
        for (int k: keysB) {
            if (!contains(k)) {
                Node *ref = nullptr;
                root = insert(root, k, &ref);
            }
        }
        rebuildSeqRandom();
    }

    bool contains(int key) const {
        Node *cur = root;
        while (cur) {
            if (key == cur->key) return true;
            cur = key < cur->key ? cur->l : cur->r;
        }
        return false;
    }

    /*────────────────────────────┐
      │  SEQUENCE OPERATIONS      │
      └──────────────────────────*/
    // merge current sequence with other (both assumed sorted ascending)
    void MERGE(const DDP &other) {
        std::vector<int> a, b;
        for (auto *p: seq) a.push_back(p->key);
        for (auto *p: other.seq) b.push_back(p->key);
        std::vector<int> res;
        std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(res));
        // rebuild from merged
        clear();
        for (int k: res) add(k);
    }

    // EXCL: remove first occurrence of subseq (contiguous) from sequence
    void EXCL(const std::vector<int> &subseq) {
        if (subseq.empty()) return;
        for (size_t i = 0; i + subseq.size() <= seq.size(); ++i) {
            bool ok = true;
            for (size_t j = 0; j < subseq.size(); ++j) if (seq[i + j]->key != subseq[j]) {
                ok = false;
                break;
            }
            if (ok) {
                seq.erase(seq.begin() + i, seq.begin() + i + subseq.size());
                rebuildTreeFromSeq();
                return;
            }
        }
    }

    // CHANGE: replace slice [pos, pos+len) with newSeq
    void CHANGE(size_t pos, size_t len, const std::vector<int> &newSeq) {
        if (pos > seq.size()) return;
        size_t end = std::min(seq.size(), pos + len);
        seq.erase(seq.begin() + pos, seq.begin() + end);
        for (size_t i = 0; i < newSeq.size(); ++i) {
            Node *ref = nullptr;
            root = insert(root, newSeq[i], &ref);
            seq.insert(seq.begin() + pos + i, ref);
        }
        rebuildTreeFromSeq();
    }

    /*────────────────────────────┐
      │  OUTPUT                   │
      └──────────────────────────*/
    void printSeq(std::ostream &os = std::cout) const {
        for (auto *p: seq) os << p->key << " ";
        os << "\n";
    }

private:
    // ascii tree pretty print (dots grid like screenshot)
    void fillMatrix(Node *n, int col, int row, int offset, std::vector<std::string> &mat, int width) const {
        if (!n) return;
        std::string numStr = std::to_string(n->key);
        for (size_t i = 0; i < numStr.size(); ++i) mat[row][col + i] = numStr[i];
        int gap = offset / 2;
        if (n->l) fillMatrix(n->l, col - gap, row + 2, gap, mat, width);
        if (n->r) fillMatrix(n->r, col + gap, row + 2, gap, mat, width);
    }

public:
    void printTree(std::ostream &os = std::cout) const {
        int h = height(root);
        if (h == 0) {
            os << "<empty>\n";
            return;
        }
        int width = (1 << h) * 2;
        int rows = h * 2;
        std::vector<std::string> mat(rows, std::string(width, '.'));
        fillMatrix(root, width / 2, 0, width / 2, mat, width);
        os << "BSTh (H=" << h << " n=" << sizeUnique() << "):\n";
        for (auto &line: mat) os << line << "\n";
    }

    /*────────────────────────────┐
      │  HELPERS                  │
      └──────────────────────────*/
    void clear() {
        // delete all nodes (simple postorder)
        std::function<void(Node *)> del = [&](Node *n) {
            if (!n)return;
            del(n->l);
            del(n->r);
            delete n;
        };
        del(root);
        root = nullptr;
        seq.clear();
    }

    ~DDP() { clear(); }

    /*────────────────────────────┐
      │  GENERATOR                │
      └──────────────────────────*/
public:
    static DDP randomObject(int power, int universe) {
        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(0, universe - 1);
        DDP obj;
        for (int i = 0; i < power; ++i) obj.add(dist(rng));
        return obj;
    }
};

//──────────────────────────────────────────────────────────────────────────────
//  Demo (можно убрать main при встраивании в библиотеку)
//──────────────────────────────────────────────────────────────────────────────
int main() {
    DDP a({1, 5, 8, 2, 7, 9, 3, 11});
    DDP b = DDP::randomObject(10, 15);

    std::cout << "A sequence: ";
    a.printSeq();
    a.printTree();

    std::cout << "B sequence: ";
    b.printSeq();
    b.printTree();

    a.unionWith(b);
    std::cout << "Union (A <- A∪B) seq: ";
    a.printSeq();
    a.printTree();

    return 0;
}
