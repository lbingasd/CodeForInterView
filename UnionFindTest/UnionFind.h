#pragma once

#include <numeric>
#include <vector>

namespace UnionFindTest {

class UnionFind {
public:
    explicit UnionFind(int n) : parent(n), rank(n, 0), components(n) { std::iota(parent.begin(), parent.end(), 0); }
    int Find(int x) { return parent[x] == x ? x : parent[x] = Find(parent[x]); }
    bool Unite(int a, int b)
    {
        a = Find(a); b = Find(b); if (a == b) return false;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a; if (rank[a] == rank[b]) ++rank[a]; --components; return true;
    }
    int Components() const { return components; }

private:
    std::vector<int> parent, rank;
    int components;
};

}
