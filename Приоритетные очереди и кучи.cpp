#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <list>

using namespace std;

class Site {
private:
    string name;
    string address;
    string launch_date;
    string language;
    string type;
    string cms;
    string last_update;
    int daily_visitors;

public:
    Site() : daily_visitors(0) {}
    Site(string n, string a, string ld, string lang, string t, string c, string lu, int dv)
        : name(n), address(a), launch_date(ld), language(lang), type(t), cms(c), last_update(lu), daily_visitors(dv) {}

    int getDailyVisitors() const { return daily_visitors; }
    string getLastUpdate() const { return last_update; }
    string getAddress() const { return address; }

    bool operator>(const Site& k) const {
        if (daily_visitors != k.daily_visitors)
            return daily_visitors > k.daily_visitors;
        if (last_update != k.last_update)
            return last_update > k.last_update;
        return address < k.address;
    }

    bool operator<(const Site& k) const {
        return !(*this > k);
    }


    bool operator==(const Site& k) const {
        return name == k.name && address == k.address && launch_date == k.launch_date &&
            language == k.language && type == k.type && cms == k.cms &&
            last_update == k.last_update && daily_visitors == k.daily_visitors;
    }

    friend ostream& operator<<(ostream& os, const Site& site) {
        os << "Site: " << site.name << ", Address: " << site.address << ", Launch Date: " << site.launch_date
            << ", Language: " << site.language << ", Type: " << site.type << ", Cms: " << site.cms
            << ", Last Update: " << site.last_update << ", Visitors: " << site.daily_visitors;
        return os;
    }
};

template<typename T>
void print_queue(T& q) {
    while (!q.empty()) {
        cout << q.top() << endl;
        q.pop();
    }
}

//узел дерева
template <class T>
class Node
{
private:
    T value;
public:
    //установить данные в узле
    Node() = default;
    Node(T v) : value(v) {}
    T getValue() { return value; }
    void setValue(T v) { value = v; }
    //сравнение узлов
    int operator<(Node N)
    {
        return (value < N.getValue());
    }
    int operator>(Node N)
    {
        return (value > N.getValue());
    }
    //вывод содержимого одного узла
    void print()
    {
        cout << value;
    }
};
template <class T>
void print(Node<T>* N)
{
    cout << N->getValue() << "\n";
}
//куча (heap)
template <class T>
class Heap
{
private:
    //массив
    Node<T>* arr;
    //сколько элементов добавлено
    int len;
    //сколько памяти выделено
    int size;
public:
    //доступ к вспомогательным полям кучи и оператор индекса
    int getCapacity() { return size; }
    int getCount() { return len; }
    Node<T>& operator[](int index)
    {
        if (index < 0 || index >= len)
            ;//?
        return arr[index];
    }
    //конструктор
    Heap<T>(int MemorySize = 100)
    {
        arr = new Node<T>[MemorySize];
        len = 0;
        size = MemorySize;
    }
    void Swap(int index1, int index2)
    {
        if (index1 <= 0 || index1 >= len);
        if (index2 <= 0 || index2 >= len);
        Node<T> temp;
        temp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = temp;
    }
    void Copy(Node<T>* dest, Node<T>* source)
    {
        dest->setValue(source->getValue());
    }
    Node<T>* GetLeftChild(int index)
    {
        if (index < 0 || index * 2 >= len);
        return &arr[index * 2 + 1];
    }
    Node<T>* GetRightChild(int index)
    {
        if (index < 0 || index * 2 >= len);
        return &arr[index * 2 + 2];
    }
    Node<T>* GetParent(int index)
    {
        if (index <= 0 || index >= len);
        if (index % 2 == 0)
            return &arr[index / 2 - 1];
        return &arr[index / 2];
    }

    int GetLeftChildIndex(int index)
    {
        if (index < 0 || index * 2 >= len);
        return index * 2 + 1;
    }
    int GetRightChildIndex(int index)
    {
        if (index < 0 || index * 2 >= len);
        return index * 2 + 2;
    }
    int GetParentIndex(int index)
    {
        if (index <= 0 || index >= len);
        if (index % 2 == 0)
            return index / 2 - 1;
        return index / 2;
    }
    void SiftUp(int index = -1)
    {
        if (index == -1) index = len - 1;

        if (index == 0) return;

        int parent_index = GetParentIndex(index);

        if (arr[index] > arr[parent_index])
        {
            Swap(index, parent_index);
            SiftUp(parent_index);
        }
    }

    template <class T>
    void push(T v)
    {
        Node<T>* N = new Node<T>;
        N->setValue(v);
        push(N);
    }
    template <class T>
    void push(Node<T>* N)
    {
        if (len < size)
        {
            Copy(&arr[len], N);
            len++; SiftUp();
        }
    }

    void Straight(void(*f)(Node<T>*))
    {
        int i;
        for (i = 0; i < len; i++)
        {
            f(&arr[i]);
        }
    }
    //перебор элементов, аналогичный проходам бинарного дерева
    void InOrder(void(*f)(Node<T>*), int index = 0)
    {
        if (GetLeftChildIndex(index) < len)
            PreOrder(f, GetLeftChildIndex(index));
        if (index >= 0 && index < len)
            f(&arr[index]);
        if (GetRightChildIndex(index) < len)
            PreOrder(f, GetRightChildIndex(index));
    }

    void SiftDown(int index = 0)
    {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;

        if (len <= leftChild) return;

        int largestChild = index;

        if (arr[leftChild] > arr[largestChild])
            largestChild = leftChild;

        if (rightChild < len && (arr[rightChild] > arr[largestChild]))
            largestChild = rightChild;

        if (largestChild != index)
        {
            Swap(index, largestChild);
            SiftDown(largestChild);
        }
    }

    T ExtractMax()
    {
        if (len == 0) {
            throw out_of_range("Heap is empty");
        }
        T res = arr[0].getValue();
        Swap(0, len - 1);
        len--;

        if (len > 1) {
            SiftDown(0);
        }

        return res;
    }

    T Remove(int index) {
        if (index < 0 || index >= len) {
            throw out_of_range("Index out of range");
        }

        T res = arr[index].getValue();

        if (index == len - 1)
            len--;
        else
        {
            Swap(index, len - 1);
            len--;

            if (arr[GetParentIndex(index)] < arr[index])
                SiftUp(index);

            else if (arr[GetParentIndex(index)] > arr[index])
                SiftDown(index);
        }
        return res;
    }

    void ChangePriority(int index, T new_value) {
        if (index < 0 || index >= len) {
            throw out_of_range("Index out of range");
        }
        T old_value = arr[index].getValue();
        arr[index].setValue(new_value);
        if (new_value > old_value) {
            SiftUp(index);
        }
        else {
            SiftDown(index);
        }
    }


    template <class T> friend ostream& operator<< (ostream& stream, Heap<T> heap);
};

template <class T>
ostream& operator<< (ostream& stream, Heap<T> heap)
{
    while (heap.getCount() > 0)
        stream << heap.ExtractMax() << ";\n";

    return stream;
}

template <class T>
class FibHeap;

template <class T>
class FibNode
{
private:
    typename list<FibNode<T>*>::iterator current;
    typename list<FibNode<T>*>::iterator parent;

    list<FibNode<T>*> children; // Список содержащий указатели на дочерние узлы текущего узла.

    int degree;
    T value;

public:
    friend class FibHeap<T>;

    typename list<FibNode<T>*>::iterator getCurrent() { return current; }
    typename list<FibNode<T>*>::iterator getParent() { return parent; }
    list<FibNode<T>*> getChildren() { return children; }

    T getValue() { return value; }
};

template <class T>
class FibHeap
{
protected:
    list<FibNode<T>*> heap;
    typename list<FibNode<T>*>::iterator max;
public:

    FibHeap<T>()
    {
        heap.clear();
        max = heap.end();
    }

    virtual ~FibHeap()
    {
        if (!heap.empty())
        {
            for (typename list<FibNode<T>*>::iterator it = heap.begin(); it != heap.end(); ++it)
                delete* it;

            heap.clear();
        }
    }

    FibNode<T>* push(T value)
    {
        FibNode<T>* add = new FibNode<T>;

        add->value = value;
        add->degree = 0;
        add->children.clear();
        add->parent = add->children.end();
        add->current = heap.insert(heap.end(), add);

        if (heap.size() == 1 || (add->value > (*max)->value))
            max = add->current;

        return add;
    }

    void Merge(FibHeap& other)
    {
        heap.splice(heap.end(), other.heap);

        if ((*other.max)->value > (*max)->value)
            max = other.max;
    }

    bool isEmpty() { return heap.empty(); }

    T ExtractMax()
    {
        if (isEmpty()) throw out_of_range("Heap is empty");;

        FibNode<T>* maxNode = *max;
        T maxValue = maxNode->value;

        if (!maxNode->children.empty())
            heap.splice(heap.end(), maxNode->children);

        heap.erase(maxNode->current);
        delete maxNode;

        if (!heap.empty())
            Consolidate();
        else
            max = heap.end();

        return maxValue;
    }

    void Consolidate()
    {
        vector<FibNode<T>*> degreeTable(heap.size() + 1, nullptr);

        typename list<FibNode<T>*>::iterator it = heap.begin();
        while (it != heap.end())
        {
            FibNode<T>* x = *it;
            int degree = x->degree;

            while (degreeTable[degree] != nullptr)
            {
                FibNode<T>* y = degreeTable[degree];
                if (x->value < y->value)
                    swap(x, y);

                Link(x, y);

                degreeTable[degree] = nullptr;
                degree++;
            }
            degreeTable[degree] = x;
            ++it;
        }

        heap.clear();
        max = heap.end();
        for (int i = 0; i < degreeTable.size(); ++i)
        {
            if (degreeTable[i] != nullptr)
            {
                degreeTable[i]->current = heap.insert(heap.end(), degreeTable[i]);
                if (max == heap.end() || degreeTable[i]->value > (*max)->value)
                    max = degreeTable[i]->current;
            }
        }
    }

    void Link(FibNode<T>* parent, FibNode<T>* child)
    {
        parent->children.push_back(child);

        if (!child->children.empty())
            parent->children.splice(parent->children.end(), child->children);

        parent->degree += child->degree;
        child->children.clear();
        parent->degree++;
    }

    void print()
    {
        int Size = heap.size();

        for (int i = 0; i < Size; i++)
            cout << ExtractMax() << ";\n";
    }
};



int main() {
    Site site1("Example1", "www.example1.com", "2020-01-01", "English", "Blog", "WordPress", "2023-01-01", 1000);
    Site site2("Example2", "www.example2.com", "2021-02-02", "English", "E-commerce", "Shopify", "2023-02-02", 1500);
    Site site3("Example3", "www.example3.com", "2022-03-03", "Spanish", "Portfolio", "Joomla", "2023-03-03", 500);

    priority_queue<Site> q;

    q.push(site1);
    q.push(site2);
    q.push(site3);

    cout << "Sites in priority order" << endl;
    print_queue(q);

    Heap<Site> heap;

    heap.push(site1);
    heap.push(site2);
    heap.push(site3);

    cout << "\nHeap before deleting\n\n";

    cout << heap;

    heap.Remove(1);

    cout << "\nHeap after deleting\n\n";

    cout << heap;

    FibHeap<Site> fibheap;

    fibheap.push(site1);
    fibheap.push(site2);
    fibheap.push(site3);

    cout << "\nExtractMax\n\n";

    while (!fibheap.isEmpty()) {
        cout << fibheap.ExtractMax() << endl;
    }

    return 0;
}
