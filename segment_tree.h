#ifndef PRUEBA2_SEGMENT_TREE_H
#define PRUEBA2_SEGMENT_TREE_H

#include <vector>
using namespace std;

class segment_tree {
    vector<int> tree;
    int size_og = 1, size = 1;

    void build(const vector<int>& nums, int idx, int lx, int rx) {
        if(lx == rx) {
            tree[idx] = nums[lx];
            return;
        }
        int mid = (lx + rx)/2;
        build(nums, idx*2 + 1, lx, mid);
        build(nums, idx*2 + 2, mid + 1, rx);
        tree[idx] = tree[idx*2 + 1] + tree[idx*2+2];
    }

    int query(int l, int r, int idx, int lx, int rx) {
        if(rx < l || r < lx){return 0;}
        if(l <= lx && rx <= r){return tree[idx];}
        int mid = (lx + rx)/2;
        int left = query(l, r, idx*2 + 1, lx, mid);
        int right = query(l, r, idx*2 + 2, mid + 1, rx);
        return left+right;
    }

    void update(int pos, int val, int idx, int lx, int rx) {
        if(lx == rx){tree[idx] = val;}
        else {
            int mid = (lx + rx)/2;
            if(pos <= mid){update(pos, val, idx*2 + 1, lx, mid);}
            else {update(pos, val, idx*2 + 2, mid + 1, rx);}
            tree[idx] = tree[idx*2 + 1] + tree[idx*2 + 2];
        }
    }

public:
    explicit segment_tree(const vector<int>& nums) {
        size_og = nums.size();
        while (size < nums.size()) size *= 2;
        tree.resize(2*size - 1, 0);
        build(nums, 0, 0, size_og - 1);
    }

    int query(int l, int r) {
        return query(l, r, 0, 0, size_og - 1);
    }

    void update(int pos, int val) {
        update(pos, val, 0, 0, size_og - 1);
    }

    void eliminate(int pos) {
        update(pos, 0, 0, 0, size_og - 1);
    }

    const vector<int>& getTree() const { return tree; }
    int getOriginalSize() const { return size_og; }
};

#endif //PRUEBA2_SEGMENT_TREE_H