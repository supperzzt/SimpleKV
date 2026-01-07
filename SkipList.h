#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <cstring>

// 跳表节点
template<typename K, typename V>
struct Node {
    K key;
    V value;
    int level; // 节点所在层级
    // forward[i] 表示第 i 层的下一个节点指针
    std::vector<std::shared_ptr<Node<K, V>>> forward;

    Node(K k, V v, int l) : key(k), value(v), level(l), forward(l + 1) {}
};

template<typename K, typename V>
class SkipList {
public:
    SkipList(int max_level = 16, float p = 0.5);
    ~SkipList() = default;

    // 核心接口
    void put(const K& key, const V& value);
    bool get(const K& key, V& value);
    void del(const K& key);
    
    // 辅助接口：打印跳表结构
    void display();

private:
    int random_level(); // 生成随机层高

    int max_level; // 最大层高
    float probability; // 提升层级的概率
    int current_level; // 当前最大层高
    std::shared_ptr<Node<K, V>> head; // 头节点
};

template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level, float p) 
    : max_level(max_level), probability(p), current_level(0) {
    // 头节点 key/value 不重要，层高必须是 max_level
    head = std::make_shared<Node<K, V>>(K(), V(), max_level);
}

// 生成随机层高，模拟抛硬币
template<typename K, typename V>
int SkipList<K, V>::random_level() {
    int level = 0;
    // 使用随机数生成器
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0, 1);

    while (dis(gen) < probability && level < max_level) {
        level++;
    }
    return level;
}

template<typename K, typename V>
void SkipList<K, V>::put(const K& key, const V& value) {
    std::vector<std::shared_ptr<Node<K, V>>> update(max_level + 1);
    auto current = head;

    // 1. 寻找插入位置
    for (int i = current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current; // 记录每层的前驱节点
    }

    // 2. 检查 Key 是否已存在
    current = current->forward[0];
    if (current && current->key == key) {
        current->value = value; // 更新值
        return;
    }

    // 3. 插入新节点
    int new_level = random_level();
    if (new_level > current_level) {
        for (int i = current_level + 1; i <= new_level; i++) {
            update[i] = head; // 如果新层高超过当前层高，需要更新头节点
        }
        current_level = new_level;
    }

    auto new_node = std::make_shared<Node<K, V>>(key, value, new_level);
    for (int i = 0; i <= new_level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }
}

template<typename K, typename V>
bool SkipList<K, V>::get(const K& key, V& value) {
    auto current = head;
    for (int i = current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
    }
    current = current->forward[0];

    if (current && current->key == key) {
        value = current->value;
        return true;
    }
    return false;
}

template<typename K, typename V>
void SkipList<K, V>::del(const K& key) {
    std::vector<std::shared_ptr<Node<K, V>>> update(max_level + 1);
    auto current = head;

    for (int i = current_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current && current->key == key) {
        // 逐层删除
        for (int i = 0; i <= current_level; i++) {
            if (update[i]->forward[i] != current) break;
            update[i]->forward[i] = current->forward[i];
        }
        // 降低层高
        while (current_level > 0 && head->forward[current_level] == nullptr) {
            current_level--;
        }
    }
}

template<typename K, typename V>
void SkipList<K, V>::display() {
    std::cout << "\n***** Skip List *****" << std::endl;
    for (int i = current_level; i >= 0; i--) {
        auto node = head->forward[i];
        std::cout << "Level " << i << ": ";
        while (node) {
            std::cout << node->key << ":" << node->value << " -> ";
            node = node->forward[i];
        }
        std::cout << "nullptr" << std::endl;
    }
}

#endif
