#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Node {
public:
    char label;
    Node* left;
    Node* right;

    Node() : label('\0'), left(nullptr), right(nullptr) {}
    ~Node() {
        delete left;
        delete right;
    }
};

template <typename T>
class STACK {
public:
    STACK() {}
    void push(const T& item) {
        data.push_back(item);
    }
    T pop() {
        if (data.empty()) {
            throw std::out_of_range("Stack is empty");
        }
        T item = data.back();
        data.pop_back();
        return item;
    }
    bool empty() const {
        return data.empty();
    }
private:
    std::vector<T> data;
};

class Tree {
public:
    Tree();
    ~Tree();
    void MakeTree();
    void OutTree();
    void DFS();
    int GetLeftSubtreeHeight();

private:
    Node* root;
    void MakeTree(Node*& node, int depth);
    void OutTree(Node* node, int x, int y, char screen[][120], int maxrow, int spacing);
    int GetHeight(Node* node);
    void LabelNodes(Node* node, char& currentLabel);

    // Disable copy and move operations
    Tree(const Tree&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree& operator=(Tree&&) = delete;
};

Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    delete root;
}

void Tree::MakeTree() {
    MakeTree(root, 1);
    char currentLabel = 'a';
    LabelNodes(root, currentLabel);
}

void Tree::MakeTree(Node*& node, int depth) {
    if (depth > 6)
        return;

    if (depth == 1 || depth <= rand() % 6 + 1) {
        node = new Node();

        MakeTree(node->left, depth + 1);
        MakeTree(node->right, depth + 1);
    } else {
        node = nullptr;
    }
}

void Tree::LabelNodes(Node* node, char& currentLabel) {
    if (node == nullptr)
        return;
    LabelNodes(node->left, currentLabel);
    LabelNodes(node->right, currentLabel);
    node->label = currentLabel++;
    if (currentLabel > 'z')
        currentLabel = 'a';
}

void Tree::OutTree() {
    if (root == nullptr) {
        std::cout << "Tree is empty!" << std::endl;
        return;
    }

    const int maxrow = 6;  // Adjusted height
    const int maxcol = 120; // Wider width
    char screen[maxrow][maxcol];

    // Initialize the screen with dots (.)
    for (int i = 0; i < maxrow; ++i) {
        for (int j = 0; j < maxcol; ++j) {
            screen[i][j] = '.';
        }
        screen[i][maxcol - 1] = '\0'; // Null-terminate each row
    }

    // Start rendering the tree with a large initial spacing
    OutTree(root, maxcol / 2, 0, screen, maxrow, maxcol / 4); // Start with wide spacing

    // Print the screen
    for (int i = 0; i < maxrow; ++i) {
        std::cout << screen[i] << std::endl;
    }
}

void Tree::OutTree(Node* node, int x, int y, char screen[][120], int maxrow, int spacing) {
    if (node == nullptr || y >= maxrow || x < 0 || x >= 120)
        return;

    // Place the current node's label
    screen[y][x] = node->label;

    // Adjust spacing dynamically: halve the spacing for each level, but keep it at least 2
    int nextSpacing = std::max(spacing / 2, 2);

    // Handle left child
    if (node->left) {
        OutTree(node->left, x - spacing, y + 1, screen, maxrow, nextSpacing);
    }

    // Handle right child
    if (node->right) {
        OutTree(node->right, x + spacing, y + 1, screen, maxrow, nextSpacing);
    }
}

void Tree::DFS() {
    if (root == nullptr) {
        std::cout << "Tree is empty!" << std::endl;
        return;
    }

    STACK<Node*> stack;
    stack.push(root);

    std::cout << "DFS traversal: ";
    while (!stack.empty()) {
        Node* node = stack.pop();
        std::cout << node->label << ' ';

        // Push right child first to process the left child first
        if (node->right)
            stack.push(node->right);
        if (node->left)
            stack.push(node->left);
    }
    std::cout << std::endl;
}

int Tree::GetLeftSubtreeHeight() {
    if (root == nullptr || root->left == nullptr)
        return 0;

    return GetHeight(root->left);
}

int Tree::GetHeight(Node* node) {
    if (node == nullptr)
        return 0;
    int leftHeight = GetHeight(node->left);
    int rightHeight = GetHeight(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Tree tree;
    tree.MakeTree();

    tree.OutTree();

    tree.DFS();

    int leftHeight = tree.GetLeftSubtreeHeight();
    std::cout << "Height of the left subtree of the root: " << leftHeight << std::endl;

    return 0;
}
