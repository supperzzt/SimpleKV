#ifndef WAL_H
#define WAL_H

#include <string>
#include <fstream>
#include <iostream>
#include <mutex>

// 预写日志：保证数据持久化
// 格式：[Operation Type 1B][Key Size 4B][Key][Value Size 4B][Value]
class WAL {
public:
    WAL(const std::string& filename) : filename_(filename) {
        log_file_.open(filename, std::ios::app | std::ios::binary);
    }

    ~WAL() {
        if (log_file_.is_open()) {
            log_file_.close();
        }
    }

    // 写入日志
    void Append(const std::string& key, const std::string& value, bool is_delete = false) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!log_file_.is_open()) return;

        char type = is_delete ? 1 : 0; // 0: Put, 1: Delete
        int key_len = key.size();
        int val_len = value.size();

        log_file_.write(&type, sizeof(char));
        log_file_.write(reinterpret_cast<char*>(&key_len), sizeof(int));
        log_file_.write(key.data(), key_len);
        
        if (!is_delete) {
            log_file_.write(reinterpret_cast<char*>(&val_len), sizeof(int));
            log_file_.write(value.data(), val_len);
        }

        log_file_.flush(); // 确保刷入磁盘
    }

    // 恢复数据（这里只是定义接口，具体要在 SimpleKV 类中实现）
    // 通常是在启动时读取文件，解析并重新 Put 到 MemTable
    void Recover() {
        // TODO: 实现日志回放逻辑
        std::cout << "Recovering from WAL: " << filename_ << std::endl;
    }

private:
    std::string filename_;
    std::ofstream log_file_;
    std::mutex mutex_;
};

#endif
