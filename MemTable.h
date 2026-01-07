#ifndef MEMTABLE_H
#define MEMTABLE_H

#include "SkipList.h"
#include <string>

// MemTable 实际上就是对 SkipList 的一层封装
// 使得接口更符合 KV 存储的习惯
class MemTable {
public:
    MemTable() : skipList(18, 0.5) {} // 默认最大层数18，概率0.5

    void Put(const std::string& key, const std::string& value) {
        skipList.put(key, value);
    }

    bool Get(const std::string& key, std::string& value) {
        return skipList.get(key, value);
    }

    void Del(const std::string& key) {
        skipList.del(key);
    }
    
    // 调试用
    void Print() {
        skipList.display();
    }

private:
    SkipList<std::string, std::string> skipList;
};

#endif
