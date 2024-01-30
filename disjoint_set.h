//
// Created by iansg on 1/29/2024.
//

#ifndef ADS_HW4_DISJOINT_SET_H
#define ADS_HW4_DISJOINT_SET_H

#include <unordered_map>
#include <stdexcept>

template<typename T>
class DisjointSet {
public:
 virtual ~DisjointSet();
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
            newDs[sz] = -1;

            delete[] ds;
            ds = newDs;
        }
        ++sz;
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

#endif //ADS_HW4_DISJOINT_SET_H
