# SimpleKV

SimpleKV 是一个基于 **LSM-Tree (Log-Structured Merge-Tree)** 架构的高性能键值存储引擎，采用 C++11 实现。

本项目参考了 LevelDB 和 RocksDB 的核心设计，旨在展示 KV 存储引擎的底层原理，包括 **SkipList（跳表）**、**WAL（预写日志）**、**SSTable（排序字符串表）** 以及 **Bloom Filter（布隆过滤器）** 等关键技术。

## 核心特性 (Features)

*   **高性能写入**：采用 **SkipList** 作为内存表 (MemTable) 的底层数据结构，支持 $O(\log N)$ 复杂度的并发写入与查询。
*   **数据持久化**：实现了 **WAL (Write-Ahead Log)** 机制，保证数据在系统崩溃后不丢失。
*   **分层存储架构**：(开发中) 实现了 MemTable 到 SSTable 的 Flush 机制。
*   **查询优化**：(开发中) 集成布隆过滤器，减少无效磁盘 I/O。

## 项目结构

```
SimpleKV/
├── include/
│   ├── SkipList.h      # 跳表实现 (核心数据结构)
│   ├── MemTable.h      # 内存表封装
│   ├── Wal.h           # 预写日志实现
│   └── BloomFilter.h   # 布隆过滤器 (To Do)
├── src/                # 源文件 (To Do)
├── tests/
│   └── main.cpp        # 测试入口
├── CMakeLists.txt      # CMake 构建脚本
└── README.md           # 项目文档
```

## 快速开始

### 编译

你需要安装 CMake 和 C++ 编译器 (g++ / clang / MSVC)。

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 运行

```bash
./simple_kv_test
# 或者在 Windows 上:
# .\Debug\simple_kv_test.exe
```

## 核心代码解析

### 1. SkipList (跳表)
位于 `include/SkipList.h`。跳表是一种概率平衡的数据结构，通过多层索引实现快速查找。相比红黑树，它在并发场景下实现更简单，且支持高效的范围查询。

### 2. WAL (预写日志)
位于 `include/Wal.h`。在写入内存之前，先将操作追加写入磁盘日志文件。这样即使程序崩溃，重启时也可以通过重放日志来恢复数据。

## 待办事项 (To-Do)

- [x] 实现 SkipList
- [x] 实现 MemTable
- [x] 实现 WAL (Write-Ahead Log)
- [ ] 实现 SSTable 序列化与反序列化
- [ ] 实现 Bloom Filter
- [ ] 实现 Level-0 到 Level-1 的 Compaction

## 许可证

MIT License
