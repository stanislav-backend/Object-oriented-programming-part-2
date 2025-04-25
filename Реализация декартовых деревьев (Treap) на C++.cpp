#include <algorithm>
#include <iostream>

using namespace std;

template <typename T> class TreapNode;
template <typename T> class Treap;
template <typename T> class ImplicitTreapNode;
template <typename T> class ImplicitTreap;
template <typename T> class TreapIterator;

template <typename T> class TreapNode {
private:
    T value;
    int priority;
    TreapNode* left = nullptr, * right = nullptr, * parent = nullptr;

public:
    TreapNode(T value, int priority, TreapNode* left = nullptr,
        TreapNode* right = nullptr, TreapNode* parent = nullptr)
        : value(value), priority(priority), left(left), right(right),
        parent(parent) {}

    TreapNode(TreapNode* node) { TreapNode(node->value, node->priority); }

    virtual ~TreapNode() {
        delete left;
        delete right;
        left = right = parent = nullptr;
    }

    TreapNode* successor() {
        TreapNode* currNode;
        if (right != nullptr) {
            for (currNode = right; currNode->left != nullptr;
                currNode = currNode->left) {
            }
            return currNode;
        }
        if (parent == nullptr)
            return nullptr;
        for (currNode = parent;
            currNode->parent != nullptr && currNode->value < value;
            currNode = currNode->parent) {
        }
        return currNode->value > value ? currNode : nullptr;
    }

    TreapNode* predecessor() {
        TreapNode* Current;
        if (left != nullptr) {
            for (Current = left; Current->right != nullptr;
                Current = Current->right) {
            }
            return Current;
        }
        if (parent == nullptr)
            return nullptr;
        for (Current = parent;
            Current->parent != nullptr && Current->value > value;
            Current = Current->parent) {
        }
        return Current->value < value ? Current : nullptr;
    }

    TreapNode* min() {
        if (!left)
            return this;
        return left->min();
    }

    TreapNode* max() {
        if (!right)
            return this;
        return right->max();
    }

    friend class Treap<T>;
    friend class ImplicitTreapNode<T>;
    friend class ImplicitTreap<T>;
    friend class TreapIterator<T>;
};

template <typename T> class Treap {
protected:
    TreapNode<T>* root = nullptr;
    
    pair<TreapNode<T>*, TreapNode<T>*> split(TreapNode<T>* node,
        const T& key) {
        if (!node)
            return { nullptr, nullptr };
        Treap<T> tmpTreap = Treap<T>(node);
        TreapIterator<T> it = tmpTreap.begin();
        TreapNode<T>* left = new TreapNode<T>(it.ptr);
        while (++it != tmpTreap.end() && *it < key)
            left = merge(left, it.ptr);
        TreapNode<T>* right = new TreapNode<T>(it.ptr);
        if (it == tmpTreap.end())
            return { left, right };
        while (++it != tmpTreap.end())
            right = merge(right, it.ptr);
        return { left, right };
    }

    TreapNode<T>* merge(TreapNode<T>* left, TreapNode<T>* right) {
        if (!left)
            return right;
        if (!right)
            return left;

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            if (left->right != nullptr)
                left->right->parent = left;
            return left;
        }
        else {
            right->left = merge(left, right->left);
            if (right->left != nullptr)
                right->left->parent = right;
            return right;
        }
    }

    explicit Treap(TreapNode<T>* root) : root(root) {}

public:
    Treap() { root = nullptr; }

    Treap(const Treap& it) = delete;

    Treap(Treap&& it) {
        root = it.root;
        it.root = nullptr;
    }

    Treap(initializer_list<T> values) : root(nullptr) {
        for (auto value : values) {
            auto [a, b] = split(root, value);
            root = merge(merge(a, new TreapNode<T>(value, rand(), nullptr,
                nullptr, nullptr)),
                b);
        }
    }

    Treap merge(Treap& another) {
        auto newRoot = merge(root, another.root);
        this->root = nullptr;
        another.root = nullptr;
        return Treap(newRoot);
    }

    pair<Treap, Treap> split(const T& key) {
        auto [a, b] = split(root, key);
        this->root = nullptr;
        a->parent = nullptr;
        b->parent = nullptr;
        return { std::move(Treap(a)), std::move(Treap(b)) };
    }

    T min() { return root->min()->value; }

    T max() { return root->max()->value; }

    TreapIterator<T> begin() {
        if (root)
            return TreapIterator(root->min());
        else
            return TreapIterator<T>(nullptr);
    }
    TreapIterator<T> end() { return TreapIterator<T>(nullptr); }

    virtual ~Treap() { delete root; }

    friend class TreapIterator<T>;
};

template <typename T>
class TreapIterator : public std::iterator<std::input_iterator_tag, const T> {
private:
    TreapNode<T>* ptr;

public:
    TreapIterator() { ptr = nullptr; }

    explicit TreapIterator(TreapNode<T>* p) { ptr = p; }

    TreapIterator(const TreapIterator& it) { ptr = it.ptr; }

    TreapIterator& operator=(const TreapIterator& it) {
        if (this == &it)
            return *this;
        ptr = it.ptr;
        return *this;
    }

    TreapIterator& operator=(TreapNode<T>* p) {
        ptr = p;
        return *this;
    }

    T& operator*() { return ptr->value; }

    T* operator->() { return &ptr->value; }

    TreapIterator& operator++() {
        if (ptr == nullptr)
            throw out_of_range("TreapIterator is out of bounds");
        ptr = ptr->successor();
        return *this;
    }

    TreapIterator operator++(int) {
        if (ptr == nullptr)
            throw out_of_range("TreapIterator is out of bounds");
        TreapIterator it = *this;
        ptr = ptr->successor();
        return it;
    }

    TreapIterator& operator--() {
        if (ptr == nullptr)
            throw out_of_range("TreeIterator is out of bounds");
        ptr = ptr->predecessor();
        return *this;
    }

    TreapIterator operator--(int) {
        if (ptr == nullptr)
            throw out_of_range("TreeIterator is out of bounds");
        TreapIterator it = *this;
        ptr = ptr->predecessor();
        return it;
    }

    bool operator!=(TreapIterator const& it) const { return ptr != it.ptr; }

    bool operator==(TreapIterator const& it) const { return ptr == it.ptr; }

    friend class Treap<T>;
    friend class ImplicitTreap<T>;
};

template <typename T> class ImplicitTreapNode : public TreapNode<T> {
private:
    int size;
    T product;

public:
    ImplicitTreapNode(const int size, const T& product, T value, int priority,
        ImplicitTreapNode* left = nullptr,
        ImplicitTreapNode* right = nullptr,
        ImplicitTreapNode* parent = nullptr)
        : TreapNode<T>(value, priority, left, right, parent), size(size),
        product(product) {}

    ImplicitTreapNode(ImplicitTreapNode* node)
        : TreapNode<T>(node->value, node->priority), size(node->size),
        product(node->product) {};

    T getProduct() { return product; }

    friend class ImplicitTreap<T>;
};

template <typename T> class ImplicitTreap : protected Treap<T> {
protected:
    void postponeRecalculation(ImplicitTreapNode<T>* node) {
        if (!node)
            return;
    }

    int size(TreapNode<T>* node) {
        if (auto typedNode = dynamic_cast<ImplicitTreapNode<T> *>(node))
            return typedNode->size;
        return 0;
    }

    T Product(int leftIndex, int rightIndex) {
        auto [left, middle] = split(typedRoot(), leftIndex); 
        auto [segment, right] = split(middle, rightIndex - leftIndex + 1);
        T result = segment ? segment->product : T();
        root = merge(merge(left, segment), right);
        return result;
    }

    void recalculate(ImplicitTreapNode<T>* node) {
        if (!node)
            return;
        postponeRecalculation(dynamic_cast<ImplicitTreapNode<T> *>(node->left));
        postponeRecalculation(
            dynamic_cast<ImplicitTreapNode<T> *>(node->right));
        node->size = 1 + size(node->left) + size(node->right);
        node->product = node->value;
        if (node->left)
            node->product *= product(node->left);
        if (node->right)
            node->product *= product(node->right);
    }

    pair<ImplicitTreapNode<T>*, ImplicitTreapNode<T>*>
        split(ImplicitTreapNode<T>* node, int index) {
        if (!node)
            return { nullptr, nullptr };
        postponeRecalculation(node);
        Treap<T> tmpITreap = ImplicitTreap<T>(node);
        TreapIterator<T> it = tmpITreap.begin();
        ImplicitTreapNode<T>* left = new ImplicitTreapNode<T>(
            dynamic_cast<ImplicitTreapNode<T> *>(it.ptr));
        while (++it != tmpITreap.end() && size(it.ptr) < index) {
            left = merge(left, dynamic_cast<ImplicitTreapNode<T> *>(it.ptr));
        }
        recalculate(left);
        ImplicitTreapNode<T>* right = new ImplicitTreapNode<T>(
            dynamic_cast<ImplicitTreapNode<T> *>(it.ptr));
        if (it == tmpITreap.end())
            return { left, right };
        while (++it != tmpITreap.end()) {
            right = merge(right, dynamic_cast<ImplicitTreapNode<T> *>(it.ptr));
        }
        recalculate(right);
        return { left, right };
    }

    ImplicitTreapNode<T>* merge(ImplicitTreapNode<T>* left,
        ImplicitTreapNode<T>* right) {
        if (!left)
            return right;
        if (!right)
            return left;
        postponeRecalculation(left);
        postponeRecalculation(right);
        if (left->priority > right->priority) {
            left->right =
                merge(dynamic_cast<ImplicitTreapNode<T> *>(left->right),
                    dynamic_cast<ImplicitTreapNode<T> *>(right));
            if (left->right != nullptr)
                left->right->parent = left;
            recalculate(left);
            return left;
        }
        else {
            right->left =
                merge(dynamic_cast<ImplicitTreapNode<T> *>(left),
                    dynamic_cast<ImplicitTreapNode<T> *>(right->left));
            if (right->left != nullptr)
                right->left->parent = right;
            recalculate(right);
            return right;
        }
    }

    ImplicitTreapNode<T>* typedRoot() {
        return dynamic_cast<ImplicitTreapNode<T> *>(this->root);
    }

public:
    ImplicitTreap() : Treap<T>() {}

    explicit ImplicitTreap(ImplicitTreapNode<T>* root) : Treap<T>(root) {}

    ImplicitTreap(initializer_list<T> values) {
        for (auto value : values) {
            this->root = merge(
                typedRoot(), new ImplicitTreapNode<T>(1, value, value, rand()));
        }
    }

    ImplicitTreap merge(ImplicitTreap& another) {
        ImplicitTreapNode<T> newRoot = merge(typedRoot(), another.typedRoot());
        return ImplicitTreap(std::move(newRoot));
    }


    pair<ImplicitTreap, ImplicitTreap> splitByIndex(int index) {
        auto [a, b] = split(typedRoot(), index);
        return { ImplicitTreap(a), ImplicitTreap(b) };
    }

    int size() { return typedRoot()->size; }
    T Product() { return typedRoot()->product; }

    TreapIterator<T> begin() {
        if (this->root)
            return TreapIterator<T>(this->root->min());
        return TreapIterator<T>(nullptr);
    }

    TreapIterator<T> end() { return TreapIterator<T>(nullptr); }
};

int main() {
    Treap<int> Tr{ 5, 7, -2, 4, 3, 6 };

    for (auto i : Tr) {
        cout << i << " ";
    }
    cout << "\n";

    auto result = Tr.split(4);
    auto& Tr1 = result.first;
    auto& Tr2 = result.second;

    for (auto i : Tr1) {
        cout << i << " ";
    }
    cout << "\n";
    for (auto i : Tr2) {
        cout << i << " ";
    }
    cout << "\n";

    Treap<int> Tr = Tr1.merge(Tr2);
    for (auto i : Tr) {
        cout << i << " ";
    }
    cout << "\n";

    cout << "Min: " << Tr.min() << ", Max: " << Tr.max() << "\n";

    cout << "\n";


    ImplicitTreap<double> Tt{ 2, 6, -3, 5, 4, 7 };
    for (auto i : Tt) {
        cout << i << " ";
    }
    cout << "\n";

    pair<ImplicitTreap<double>, ImplicitTreap<double>> result = Tt.splitByIndex(4);
    ImplicitTreap<double> Tt1 = result.first;
    ImplicitTreap<double> Tt2 = result.second;

    for (auto i : Tt1) {
        cout << i << " ";
    }
    cout << "\n";
    for (auto i : Tt2) {
        cout << i << " ";
    }
    cout << "\n";

    cout << Tt.Product() << "\n";
    cout << Tt1.Product() << "\n";
    cout << Tt2.Product() << "\n";

    return 0;
}