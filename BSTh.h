#pragma once
#include <iostream>
#include <string>
#include <clocale>
#include <stack>
#include <iterator>
#include <algorithm>

using namespace std;

struct Node {
    int key;
    Node *L[2];
    int h;
    Node(int k) : key(k), h(1) { L[0] = L[1] = nullptr; }

    ~Node() {
        delete L[1];
        delete L[0];
    }

    void Display(int, int, int);

    Node(const Node &) = delete;

    Node &operator =(const Node &) = delete;

    friend int height(Node *&p) {
        return p ? p->h : 0;
    }

    int balancefactor() {
        return height(L[1]) - height(L[0]);
    }

    int fixheight() {
        auto hl = height(L[0]);
        auto hr = height(L[1]);
        h = (hl > hr ? hl : hr) + 1;
        return h;
    }
};

using MyStack = stack<pair<Node *, int> >;


struct myiter : public std::iterator<std::forward_iterator_tag, int> {
    Node *Ptr;
    MyStack St;

    myiter(Node *p = nullptr) : Ptr(p) {
    }

    myiter(Node *p, const MyStack &&St) : Ptr(p), St(move(St)) {
    }

    bool operator ==(const myiter &Other) const { return Ptr == Other.Ptr; }
    bool operator !=(const myiter &Other) const { return !(*this == Other); }

    myiter &operator++();

    myiter operator++(int) {
        myiter temp(*this);
        ++*this;
        return temp;
    }

    pointer operator->() { return &Ptr->key; }
    reference operator*() { return Ptr->key; }
};

template<typename Container, typename Iter = myiter>
class outiter : public std::iterator<std::output_iterator_tag, typename Container::value_type> {
protected:
    Container &container;
    Iter iter;

public:
    explicit outiter(Container &c, Iter it) : container(c), iter(it) {
    }


    const outiter<Container> &
    operator =(const typename Container::value_type &value) {
        iter = container.insert(value, iter).first;
        return *this;
    }

    const outiter<Container> &
    operator =(const outiter<Container> &) { return *this; }

    outiter<Container> &operator*() { return *this; }

    outiter<Container> &operator++() { return *this; }
    outiter<Container> &operator++(int) { return *this; }
};


template<typename Container, typename Iter>
inline outiter<Container, Iter> outinserter(Container &c, Iter it) {
    return outiter<Container, Iter>(c, it);
}


class Tree {
    static size_t tags;
    char tag;
    Node *root;
    size_t n;

public:
    using key_type = int;
    using value_type = int;
    using key_compare = less<int>;

    void swap(Tree &rgt) {
        std::swap(tag, rgt.tag);
        std::swap(root, rgt.root);
        std::swap(n, rgt.n);
    }

    static int Count;

    myiter Insert(const int &k,
                  myiter where) {
        return insert(k, where).first;
    }

    size_t H() { return (root ? root->h : 0); }

    void Display(int = 1);

    myiter begin() const;

    myiter end() const { return myiter(nullptr); }

    void clear() {
        n = 0;
        delete root;
        root = nullptr;
    }

    pair<myiter, bool> insert(int, myiter = myiter(nullptr));

    pair<myiter, bool> erase(int);

    Tree() : tag(static_cast<char>('A' + tags++)), root(nullptr), n(0) {
    }

    int size() { return n; }
    ~Tree() { delete root; }

    myiter find(int) const;

    Tree(const Tree &rgt) : Tree() {
        for (auto x = rgt.begin(); x != rgt.end(); ++x) insert(*x);
    }

    Tree(Tree &&rgt) : Tree() { swap(rgt); }

    template<class MyIt>
    Tree(MyIt first, MyIt last) : Tree() {
        for (; first != last; ++first) insert(*first);
    }

    Tree &operator=(const Tree &rgt) {
        Tree temp;
        for (auto x: rgt) temp.insert(x);
        swap(temp);
        return *this;
    }

    Tree &operator=(Tree &&rgt) {
        swap(rgt);
        return *this;
    }

    Tree &operator |=(const Tree &);

    Tree operator |(const Tree &rgt) const {
        Tree result(*this);
        return (result |= rgt);
    }

    Tree &operator &=(const Tree &);

    Tree operator &(const Tree &rgt) const {
        Tree result(*this);
        return (result &= rgt);
    }

    Tree &operator -=(const Tree &);

    Tree operator -(const Tree &rgt) const {
        Tree result(*this);
        return (result -= rgt);
    }

    Tree &operator ^=(const Tree &);

    Tree operator ^(const Tree &rgt) const {
        Tree result(*this);
        return (result -= rgt);
    }
};
