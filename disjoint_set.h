//
// Created by iansg on 1/29/2024.
//

#ifndef ADS_HW4_DISJOINT_SET_H
#define ADS_HW4_DISJOINT_SET_H

#include <unordered_map>
#include <vector>
#include <stdexcept>

template<typename T>
class DisjointSet {
public:
 virtual ~DisjointSet(){}
 virtual void MakeSet(T x) = 0;
 virtual T Find(T x) = 0;
 virtual void Union(T x, T y) = 0;
 virtual bool isConnected(T x, T y) = 0; // verifies if there is a path between x & y
};

template<typename T>
class DisjointSetArray : public DisjointSet<T> {
    int* ds;
    int sz = 0;
    int capacity = 1;
    std::unordered_map<T,int> key;
    std::unordered_map<int,T> value;

    int _find(int a) {
        if (ds[a] < 0) return a;
        ds[a] = _find(ds[a]);
        return ds[a];
    }
    void _union(int a, int b) {
        a = _find(a);
        b = _find(b);

        if (a == b) return;
        if (ds[a] == ds[b]) {
            ds[b] = a;
            --ds[a];
        }
        else if (ds[a] < ds[b]) ds[b] = a;
        else ds[a] = b;
    }
public:
    DisjointSetArray() {
        ds = new int[capacity];
    }
    ~DisjointSetArray() {
        delete[] ds;
    }
    void MakeSet(T a) {
        if (key.count(a)) return;

        key[a] = sz;
        value[sz] = a;
        if (sz == capacity) {
            int* newDs = new int[capacity<<=1];
            for (int i = 0; i < sz; ++i)
                newDs[i] = ds[i];

            delete[] ds;
            ds = newDs;
        }
        ds[sz++] = -1;
    }
    T Find(T a) {
        if (key.count(a) == 0)
            throw std::runtime_error("'MakeSet' must be called for an element before using 'Find'.");

        return value[_find(key[a])];
    }
    void Union(T a, T b) {
        if (key.count(a) + key.count(b) == 0)
            throw std::runtime_error("'MakeSet' must be called for an element before using 'Find'.");

        _union(key[a], key[b]);
    }
    bool isConnected(T a, T b) {
        return Find(a) == Find(b);
    }
};

template<typename T>
class DisjointSetTree : public DisjointSet<T> {
private:
    struct Node {
        T value;
        Node* parent;
        int rank;
        Node(T v): value(v), parent(this), rank(0) {}
    };
    std::unordered_map<T, Node*> _elements;
public:
    DisjointSetTree()=default;
    ~DisjointSetTree(){
        for (auto& iter: _elements) {
            delete iter.second;
        }
    }
    void MakeSet(T x) override {
        if (_elements[x]) {
            return;
        }
        auto* node = new Node(x);
        _elements[x] = node;
    }
    T Find(T x) override {
        if (!_elements[x]) {
            throw std::runtime_error("Element not in DisjoinSet");
        }
        if (_elements[x]->parent == _elements[x]){
            return x;
        }
        else {
            return Find(_elements[x]->parent->value);
        }
    }
    void Union(T x, T y) override {
        if (!_elements[x] || !_elements[y]) {
            throw std::runtime_error("Element not in DisjoinSet");
        }
        auto xroot = Find(x);
        auto yroot = Find(y);
        if (_elements[xroot] == _elements[yroot]) {
            return;
        }
        if (_elements[xroot]->rank < _elements[yroot]->rank) {
            _elements[xroot]->parent = _elements[yroot];
        }
        else if (_elements[xroot]->rank > _elements[yroot]->rank) {
            _elements[yroot]->parent = _elements[xroot];
        }
        else {
            _elements[yroot]->parent = _elements[xroot];
            _elements[xroot]->rank++;
        }
    }
    bool isConnected(T x, T y) override {
        if (!_elements[x] || !_elements[y]) {
            throw std::runtime_error("Element not in DisjoinSet");
        }
        auto xroot = Find(x);
        auto yroot = Find(y);
        return _elements[xroot] == _elements[yroot];
    }
    void printSets() {
        std::cout << "PRINTING SETS:\n";
        std::unordered_map<T, std::vector<T>> sets;
        for (const auto& iter: _elements) {
            T root = Find(iter.first);
            sets[root].push_back(iter.first);
        }
        for (const auto& iter: sets) {
            std::cout << "Root: " << iter.first << '\n';
            std::cout << "Elements: ";
            for (const auto& iter2: iter.second) {
                std::cout << iter2 << " ";
            }
            std::cout << '\n';
        }
    }
};

#endif //ADS_HW4_DISJOINT_SET_H
