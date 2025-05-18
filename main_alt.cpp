#include <bits/stdc++.h>

class DDP {
    struct Node {
        int key;
        Node *l, *r;
        int h;

        explicit Node(const int k) : key(k), l(nullptr), r(nullptr), h(1) {
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

    static Node *insert(Node *n, const int k, Node **out) {
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

    static Node *cloneTree(const Node *n) {
        if (!n) return nullptr;
        Node *m = new Node(n->key);
        m->h = n->h;
        m->l = cloneTree(n->l);
        m->r = cloneTree(n->r);
        return m;
    }

    static Node *findNode(Node *n, const int key) {
        if (!n) return nullptr;
        if (key == n->key) return n;
        return key < n->key ? findNode(n->l, key) : findNode(n->r, key);
    }

    static void fillMatrix(const Node *n, const int col, const int row, const int off, std::vector<std::string> &m) {
        if (!n)return;
        const std::string s = std::to_string(n->key);
        for (size_t i = 0; i < s.size(); ++i)m[row][col + i] = s[i];
        const int gap = off / 2;
        if (n->l)fillMatrix(n->l, col - gap, row + 2, gap, m);
        if (n->r)fillMatrix(n->r, col + gap, row + 2, gap, m);
    }

    static void clearNodes(const Node *n) {
        if (!n) return;
        clearNodes(n->l);
        clearNodes(n->r);
        delete n;
    }

    void clear() {
        clearNodes(root);
        root = nullptr;
        seq.clear();
    }

    static std::vector<int> sampleUnique(const int cnt, const int univ) {
        if (univ < cnt) throw std::invalid_argument("universe < power");
        std::vector<int> v(univ);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});
        v.resize(cnt);
        return v;
    }

public:
    DDP() = default;

    explicit DDP(const std::vector<int> &v) { for (const int k: v) add(k); }

    DDP(const DDP &other) {
        root = cloneTree(other.root);
        seq.clear();
        seq.reserve(other.seq.size());
        for (const Node *p: other.seq) {
            seq.push_back(findNode(root, p->key));
        }
    }

    DDP &operator=(const DDP &other) {
        if (this != &other) {
            DDP tmp(other);
            std::swap(root, tmp.root);
            std::swap(seq, tmp.seq);
        }
        return *this;
    }

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
    }

    void intersectWith(const DDP &o) {
        std::vector<int> ks;
        inorder(root, ks);
        for (const int k: ks)if (!o.contains(k))root = erase(root, k);
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
    }

    void MERGE(const DDP &other) {
        for (const auto *p: other.seq) {
            add(p->key);
        }
    }

    void EXCL(const DDP &sub) {
        const size_t n = sub.seq.size();
        if (n == 0 || n > seq.size())
            throw std::runtime_error("Subsequence not found");

        for (size_t i = 0; i + n <= seq.size(); ++i) {
            bool ok = true;
            for (size_t j = 0; j < n; ++j)
                if (seq[i + j]->key != sub.seq[j]->key) {
                    ok = false;
                    break;
                }
            if (ok) {
                seq.erase(seq.begin() + i, seq.begin() + i + n);
                rebuildTree();
                return;
            }
        }
        throw std::runtime_error("Subsequence not found");
    }

    void CHANGE(const size_t pos, const DDP &repl) {
        if (pos >= seq.size())
            throw std::out_of_range("Position is outside the sequence");

        std::vector<int> keys;
        keys.reserve(seq.size());
        for (const auto *p: seq) keys.push_back(p->key);

        const size_t eraseLen = std::min(repl.seq.size(), keys.size() - pos);
        if (eraseLen == 0 && repl.seq.empty())
            throw std::runtime_error("Nothing to change: both erase length and replacement are empty");
        keys.erase(keys.begin() + pos, keys.begin() + pos + eraseLen);
        std::vector<int> replKeys;
        replKeys.reserve(repl.seq.size());
        for (const auto *p: repl.seq) replKeys.push_back(p->key);
        keys.insert(keys.begin() + pos, replKeys.begin(), replKeys.end());
        clear();
        for (const int k: keys) add(k);
    }

    void printSeq(std::ostream &os = std::cout) const {
        if (seq.empty()) throw std::runtime_error("Sequence is empty");
        for (const auto *p: seq) os << p->key << ' ';
        os << '\n';
    }

    void printTree(std::ostream &os = std::cout) const {
        const int h = height(root);
        if (!h) throw std::runtime_error("Tree is empty");
        const int w = (1 << h) * 2, rows = h * 2;
        std::vector<std::string> mat(rows, std::string(w, '.'));
        fillMatrix(root, w / 2, 0, w / 2, mat);
        os << "BSTh(H=" << h << " n=" << cardinality() << ") ----->\n";
        for (auto &ln: mat) os << ln << '\n';
    }

    ~DDP() { clear(); }

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
    freopen("in.txt", "w", stdout);
    for (int len = 10; len <= 200; len += 1) {
        for (int i = 0; i < 1; ++i) {
            const int univ = len * 3 / 2;
            int ans = 0;
            DDP A = DDP::genUniqueSet(len, univ);
            DDP B = DDP::genUniqueSet(len, univ);
            DDP C = DDP::genUniqueSet(len, univ);
            DDP D = DDP::genUniqueSet(len, univ);
            DDP E = DDP::genUniqueSet(len, univ);

            DDP M1 = DDP::genSequence(len, univ);
            DDP M2 = DDP::genSequence(len, univ);

            DDP CH1 = DDP::genSequence(len, univ);
            DDP CH2 = DDP::genSequence(len / 2, univ);

            constexpr int ind = 2;
            auto start = std::chrono::high_resolution_clock::now();

            ans += A.cardinality() + B.cardinality();
            A.intersectWith(B);

            ans += A.cardinality() + C.cardinality();
            A.unionWith(C);

            ans += D.cardinality() + E.cardinality();
            D.symDiffWith(E);

            ans += A.cardinality() + D.cardinality();
            A.unionWith(D);

            ans += M1.cardinality() + M2.cardinality();
            M1.MERGE(M2);

            ans += CH1.cardinality() + CH2.cardinality();
            CH1.CHANGE(ind, CH2);

            ans += CH1.cardinality() + CH2.cardinality();
            CH1.EXCL(CH2);

            auto stop = std::chrono::high_resolution_clock::now();
            cout << ans / 14 << " " << std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count() << "\n";
        }
    }
    return 0;
}
