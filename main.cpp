#include <bits/stdc++.h>

class DDP {
    struct Node {
        int key;
        Node *l, *r;
        int h;

        explicit Node(const int k): key(k), l(nullptr), r(nullptr), h(1) {
        }
    };

    Node *root = nullptr;
    std::vector<Node *> seq;

    static int height(const Node *n) { return n ? n->h : 0; }

    static void upd(Node *n) {
        if (!n)return;
        n->h = std::max(height(n->l), height(n->r)) + 1;
    }

    static Node *rotR(Node *y) {
        Node *x = y->l;
        Node *t = x->r;
        x->r = y;
        y->l = t;
        upd(y);
        upd(x);
        return x;
    }

    static Node *rotL(Node *x) {
        Node *y = x->r;
        Node *t = y->l;
        y->l = x;
        x->r = t;
        upd(x);
        upd(y);
        return y;
    }

    static int bal(const Node *n) { return n ? height(n->l) - height(n->r) : 0; }

    static Node *insert(Node *n, int k, Node **out) {
        if (!n) {
            *out = new Node(k);
            return *out;
        }
        if (k < n->key) n->l = insert(n->l, k, out);
        else if (k > n->key) n->r = insert(n->r, k, out);
        else {
            *out = n;
            return n;
        }
        upd(n);
        const int bf = bal(n);
        if (bf > 1 && k < n->l->key) return rotR(n);
        if (bf < -1 && k > n->r->key) return rotL(n);
        if (bf > 1 && k > n->l->key) {
            n->l = rotL(n->l);
            return rotR(n);
        }
        if (bf < -1 && k < n->r->key) {
            n->r = rotR(n->r);
            return rotL(n);
        }
        return n;
    }

    static Node *minVal(Node *n) {
        while (n->l)n = n->l;
        return n;
    }

    static Node *erase(Node *n, const int k) {
        if (!n) return nullptr;
        if (k < n->key) n->l = erase(n->l, k);
        else if (k > n->key) n->r = erase(n->r, k);
        else {
            if (!n->l || !n->r) {
                Node *t = n->l ? n->l : n->r;
                delete n;
                return t;
            }
            const Node *s = minVal(n->r);
            n->key = s->key;
            n->r = erase(n->r, s->key);
        }
        upd(n);
        const int bf = bal(n);
        if (bf > 1 && bal(n->l) >= 0) return rotR(n);
        if (bf > 1 && bal(n->l) < 0) {
            n->l = rotL(n->l);
            return rotR(n);
        }
        if (bf < -1 && bal(n->r) <= 0) return rotL(n);
        if (bf < -1 && bal(n->r) > 0) {
            n->r = rotR(n->r);
            return rotL(n);
        }
        return n;
    }

    static void inorder(const Node *n, std::vector<int> &v) {
        if (!n)return;
        inorder(n->l, v);
        v.push_back(n->key);
        inorder(n->r, v);
    }

    void rebuildTree() {
        root = nullptr;
        for (Node *&p: seq) {
            Node *ref = nullptr;
            root = insert(root, p->key, &ref);
            p = ref;
        }
    }

    void rebuildSeqRandom() {
        std::vector<int> keys;
        inorder(root, keys);
        std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
        seq.clear();
        for (const int k: keys) {
            Node *ref = nullptr;
            insert(root, k, &ref);
            seq.push_back(ref);
        }
    }

public:
    DDP() = default;

    explicit DDP(const std::vector<int> &v) { for (const int k: v)add(k); }

    void add(const int k) {
        Node *ref = nullptr;
        root = insert(root, k, &ref);
        seq.push_back(ref);
    }

    void removeOne(const int k) {
        const auto it = std::find_if(seq.begin(), seq.end(), [&](const Node *n) { return n->key == k; });
        if (it == seq.end())return;
        seq.erase(it);
        if (std::none_of(seq.begin(), seq.end(), [&](const Node *n) { return n->key == k; })) root = erase(root, k);
    }

    size_t cardinality() const {
        std::vector<int> v;
        inorder(root, v);
        return v.size();
    }

    bool contains(const int k) const {
        const Node *c = root;
        while (c) {
            if (k == c->key)return true;
            c = k < c->key ? c->l : c->r;
        }
        return false;
    }

    void unionWith(const DDP &o) {
        std::vector<int> ks;
        inorder(o.root, ks);
        for (const int k: ks) {
            Node *r = nullptr;
            root = insert(root, k, &r);
        }
        rebuildSeqRandom();
    }

    void intersectWith(const DDP &o) {
        std::vector<int> ks;
        inorder(root, ks);
        for (const int k: ks)if (!o.contains(k))root = erase(root, k);
        rebuildSeqRandom();
    }

    void symDiffWith(const DDP &o) {
        std::vector<int> a;
        inorder(root, a);
        std::vector<int> b;
        inorder(o.root, b);
        const std::unordered_set<int> sb(b.begin(), b.end());
        for (int k: a)if (sb.count(k))root = erase(root, k);
        for (const int k: b)
            if (!contains(k)) {
                Node *r = nullptr;
                root = insert(root, k, &r);
            }
        rebuildSeqRandom();
    }

    void MERGE(const DDP &o) {
        std::vector<int> a, b;
        for (const auto *p: seq)a.push_back(p->key);
        for (const auto *p: o.seq)b.push_back(p->key);
        std::vector<int> r;
        std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(r));
        clear();
        for (const int k: r)add(k);
    }

    void EXCL(const std::vector<int> &sub) {
        if (sub.empty())return;
        for (size_t i = 0; i + sub.size() <= seq.size(); ++i) {
            bool ok = true;
            for (size_t j = 0; j < sub.size(); ++j)
                if (seq[i + j]->key != sub[j]) {
                    ok = false;
                    break;
                }
            if (ok) {
                seq.erase(seq.begin() + i, seq.begin() + i + sub.size());
                rebuildTree();
                return;
            }
        }
    }

    void CHANGE(const size_t pos, const size_t len, const std::vector<int> &ns) {
        if (pos > seq.size())return;
        const size_t end = std::min(seq.size(), pos + len);
        seq.erase(seq.begin() + pos, seq.begin() + end);
        for (size_t i = 0; i < ns.size(); ++i) {
            Node *r = nullptr;
            root = insert(root, ns[i], &r);
            seq.insert(seq.begin() + pos + i, r);
        }
        rebuildTree();
    }

    void printSeq(std::ostream &os = std::cout) const {
        for (const auto *p: seq)os << p->key << " ";
        os << "\n";
    }

    void printTree(std::ostream &os = std::cout) const {
        const int h = height(root);
        if (!h) {
            os << "<empty>\n";
            return;
        }
        const int w = (1 << h) * 2;
        const int rows = h * 2;
        std::vector<std::string> mat(rows, std::string(w, '.'));
        fillMatrix(root, w / 2, 0, w / 2, mat);
        os << "BSTh(H=" << h << " n=" << cardinality() << ") ----->\n";
        for (auto &ln: mat)os << ln << "\n";
    }

private:
    static void fillMatrix(const Node *n, const int col, const int row, const int off, std::vector<std::string> &m) {
        if (!n)return;
        const std::string s = std::to_string(n->key);
        for (size_t i = 0; i < s.size(); ++i)m[row][col + i] = s[i];
        const int gap = off / 2;
        if (n->l)fillMatrix(n->l, col - gap, row + 2, gap, m);
        if (n->r)fillMatrix(n->r, col + gap, row + 2, gap, m);
    }

    void clear() {
        std::function<void(Node *)> del = [&](const Node *n) {
            if (!n)return;
            del(n->l);
            del(n->r);
            delete n;
        };
        del(root);
        root = nullptr;
        seq.clear();
    }

public:
    ~DDP() { clear(); }

    // generator
private:
    static std::vector<int> sampleUnique(const int cnt, const int univ) {
        if (univ < cnt) throw std::invalid_argument("universe < power");
        std::vector<int> v(univ);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
        v.resize(cnt);
        return v;
    }

public:
    static DDP genUniqueSet(const int power, const int universe) {
        DDP o;
        for (const int k: sampleUnique(power, universe)) o.add(k);
        return o;
    }

    static DDP genSequence(const int len, const int universe) {
        DDP o;
        std::mt19937 rng{std::random_device{}()};
        std::uniform_int_distribution<int> dist(0, universe - 1);
        for (int i = 0; i < len; ++i) o.add(dist(rng));
        return o;
    }
};


int main() {
    using std::cout;
    cout << "============ SET OPERATIONS ============\n";
    DDP A = DDP::genUniqueSet(10, 15);
    DDP B = DDP::genUniqueSet(10, 15);
    DDP C = DDP::genUniqueSet(10, 15);
    DDP D = DDP::genUniqueSet(10, 15);
    DDP E = DDP::genUniqueSet(10, 15);

    cout << "A: ";
    A.printSeq();
    A.printTree();
    cout << "\nB: ";
    B.printSeq();
    B.printTree();
    cout << "\nC: ";
    C.printSeq();
    C.printTree();
    cout << "\nD: ";
    D.printSeq();
    D.printTree();
    cout << "\nE: ";
    E.printSeq();
    E.printTree();

    DDP T = A;
    T.intersectWith(B);
    cout << "\nA ∩ B: ";
    T.printSeq();
    T.printTree();

    T.unionWith(C);
    cout << "\n(A ∩ B) ∪ C: ";
    T.printSeq();
    T.printTree();

    DDP tmp = D;
    tmp.symDiffWith(E);
    cout << "\nD ⊕ E: ";
    tmp.printSeq();
    tmp.printTree();

    T.unionWith(tmp);
    cout << "\n((A ∩ B) ∪ C) ∪ (D ⊕ E): ";
    T.printSeq();
    T.printTree();

    cout << "\n========== SEQUENCE OPERATIONS ==========";
    // MERGE
    DDP S1 = DDP::genSequence(10, 15);
    DDP S2 = DDP::genSequence(10, 15);
    cout << "\nS1: ";
    S1.printSeq();
    S1.printTree();
    cout << "\nS2: ";
    S2.printSeq();
    S2.printTree();
    S1.MERGE(S2);
    cout << "\nS1.MERGE(S1,S2) => S1:\n";
    S1.printSeq();
    S1.printTree();

    // EXCL – удалим подпоследовательность {4,5,6}
    const std::vector<int> subseq = {4, 5, 6};
    S1.EXCL(subseq);
    cout << "\nEXCL{4,5,6} => S1:\n";
    S1.printSeq();
    S1.printTree();

    // CHANGE – на позиции 2 (0‑based) заменить 3 элемента на {99,100}
    S1.CHANGE(2, 3, {99, 100});
    cout << "\nCHANGE pos=2 len=3 -> {99,100} => S1:\n";
    S1.printSeq();
    S1.printTree();
    return 0;
}
