#include <iostream>
#include <cassert>
#include "disjoint_set.h"
using namespace std;

void test_ds_array() {
    DisjointSetArray<char> ds;
    ds.MakeSet('a');
    ds.MakeSet('z');
    ds.MakeSet('d');
    ds.MakeSet('f');
    ds.MakeSet('g');
    ds.MakeSet('h');
    assert(ds.Find('a') == 'a');
    assert(ds.Find('z') == 'z');
    assert(ds.Find('g') == 'g');
    ds.Union('a', 'z');
    ds.Union('a', 'd');
    assert(ds.Find('a') == 'a');
    assert(ds.Find('z') == 'a');
    ds.Union('f', 'g');
    ds.Union('g', 'f');
    ds.Union('g', 'h');
    assert(ds.isConnected('f', 'h') == true);
    assert(ds.isConnected('d', 'z') == true);
    ds.Union('g', 'd');
    assert(ds.Find('a') == 'f');
}

int main() {
    test_ds_array();
    return 0;
}
