#pragma once

#include "PLL_dynamic.h"
#include <set>

// 自定义优先队列数据结构，用于DIFFUSE和SPREAD3的算法
class DuQueue
{
private:
    // 优先队列比较函数（按照du由小到大）
    struct Compare
    {
        bool operator()(
                const std::pair<int, weightTYPE> &a, 
                const std::pair<int, weightTYPE> &b) const {
            if (a.second != b.second)
                return a.second < b.second || (a.second == b.second && a.first < b.first);
        }
    };
    // 用set作为优先队列容器，元素为<u, du>：du为结点u对应的路径长度
    std::set<std::pair<int, weightTYPE>, Compare> _queue;
    // 用unordered_map存储<u, du>，便于实现论文中的取Q[u]值
    std::unordered_map<int, weightTYPE> _map;

public:
    // emplace用于插入或更新结点u对应的路径长度du（如果u已在队列中）
    void emplace(int u, weightTYPE du)
    {
        auto it = _map.find(u);
        if (it != _map.end())
        {
            _queue.erase({u, it->second});
        }
        _map[u] = du;
        _queue.emplace(u, du);
    }

    std::pair<int, weightTYPE> top() const
    {
        return *_queue.begin();
    }

    void pop()
    {
        auto it = _queue.begin();
        _map.erase(it->first);
        _queue.erase(it);
    }

    bool empty() const
    {
        return _queue.empty();
    }

    // 重载[]
    weightTYPE operator[](int u)
    {
        auto it = _map.find(u);
        if (it != _map.end())
            return _map[u];
        else
            return MAX_VALUE;
    }
};