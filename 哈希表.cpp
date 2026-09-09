#include <iostream>
#include <string>
#include <vector>
#include <unordered_map> // 核心：哈希映射 (Key-Value)
#include <unordered_set> // 核心：哈希集合 (仅 Key，用于快速去重和查找)

using namespace std;

int main() {
    // 开启流加速
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=================== 1. 哈希表底层原理与基本声明 ===================\n";
    /*
     * 【底层结构与原理】：
     * 1. 散列表（Hash Table）：通过【哈希函数 (Hash Function)】把 Key 转化为数组下标（桶 Bucket）。
     * 2. 冲突解决：C++ STL 内部通常采用【拉链法（Chaining）】处理哈希碰撞。
     * 3. 复杂度：
     *    - 平均情况：插入、删除、查找均为 O(1) 极速。
     *    - 最坏情况：大量哈希冲突时退化为 O(n)。
     * 4. 无序性：与 map 不同，哈希表里的元素【完全无序】，遍历顺序由哈希值和桶分布决定。
     */

    // 声明一个哈希表：Key 为学号 (int)，Value 为姓名 (string)
    unordered_map<int, string> student_map;

    // 列表初始化
    unordered_map<string, int> fruit_prices = {
        {"Apple", 5},
        {"Banana", 3},
        {"Orange", 4}
    };


    cout << "\n=================== 2. 插入与性能优化 (reserve 防止动态扩容) ===================\n";
    /*
     * 🚀 算法竞赛/工程关键优化技巧：
     * 哈希表在元素增多时会自动扩容并重新计算哈希值（Rehash），这非常耗时。
     * 如果提前知道大概的数据量 N，务必调用 .reserve(N)，性能提升立竿见影！
     */
    student_map.reserve(1024); // 预留 1024 个桶，避免运行中反复扩容

    // 插入方式 ①：下标 [] 赋值（若 Key 不存在则新建；若已存在则【覆盖更新】）
    student_map[101] = "Alice";
    student_map[102] = "Bob";
    student_map[101] = "Alice_Update"; // 101 的值被覆盖修改

    // 插入方式 ②：insert()（若 Key 已存在则【放弃插入，不修改原值】）
    student_map.insert({103, "Charlie"});
    student_map.insert({103, "Fake_Charlie"}); // 插入失败，103 仍为 Charlie

    // 插入方式 ③：emplace()（就地构造，减少临时对象开销）
    student_map.emplace(104, "David");

    cout << "当前学生人数: " << student_map.size() << "\n";


    cout << "\n=================== 3. 查找与判断 Key 是否存在（核心规范） ===================\n";
    /*
     * ⚠️ 再次重申避坑规则：
     * 千万不要用 student_map[999] 来判断 999 是否存在！
     * 一旦访问，哈希表会莫名其妙多出一个 key=999、value="" 的脏元素！
     */

    // 规范写法 ①：使用 count(key)（判断存在最便捷）
    // 返回 1 表示存在，返回 0 表示不存在
    if (student_map.count(102)) {
        cout << "学号 102 存在，姓名是: " << student_map[102] << "\n";
    }

    if (!student_map.count(999)) {
        cout << "学号 999 不存在于表中\n";
    }

    // 规范写法 ②：使用 find(key)（需要读写值时效率最高，只寻址一次）
    auto it = student_map.find(103);
    if (it != student_map.end()) {
        cout << "通过 find 找到学号 " << it->first << " -> " << it->second << "\n";
    } else {
        cout << "未找到指定学号\n";
    }


    cout << "\n=================== 4. 遍历哈希表（观察无序特征） ===================\n";
    cout << "--- student_map 内部遍历（顺序并非按学号排序，属于正常现象） ---\n";
    // 采用 C++17 结构化绑定
    for (const auto& [id, name] : student_map) {
        cout << "学号: " << id << " \t姓名: " << name << "\n";
    }


    cout << "\n=================== 5. 删除操作 ===================\n";
    // 方式 ①：按 Key 删除（返回成功删除的个数：1 或 0）
    size_t num_erased = student_map.erase(104);
    cout << "删除了 " << num_erased << " 个学号为 104 的学生\n";

    // 方式 ②：按迭代器删除
    auto del_it = student_map.find(101);
    if (del_it != student_map.end()) {
        student_map.erase(del_it);
    }


    cout << "\n=================== 6. 底层探秘：查看哈希桶（Bucket）状态 ===================\n";
    // 帮助你直观理解“拉链法哈希表”是如何在内存中运作的
    cout << "当前桶（Bucket）总数量: " << student_map.bucket_count() << "\n";
    cout << "负载因子（元素总数 / 桶总数）: " << student_map.load_factor() << "\n";
    
    // 查看某个 Key 具体被分配到了第几个桶
    if (student_map.count(102)) {
        cout << "学号 102 被哈希函数映射到了第 " << student_map.bucket(102) << " 号桶中\n";
    }


    cout << "\n=================== 7. 哈希集合 unordered_set 简要演示 ===================\n";
    /*
     * 如果你【不需要存储 Value】，只需要：
     * 1. 元素快速去重
     * 2. O(1) 速度判断某数字/字符串是否出现过
     * 直接使用 unordered_set！它比 unordered_map 占用更少的内存。
     */
    unordered_set<string> visited_urls;
    visited_urls.insert("https://google.com");
    visited_urls.insert("https://github.com");
    visited_urls.insert("https://google.com"); // 重复插入会被自动过滤

    cout << "唯一 URL 数量: " << visited_urls.size() << " (已自动去重)\n";

    string target_url = "https://github.com";
    if (visited_urls.count(target_url)) {
        cout << "网址已访问过: " << target_url << "\n";
    }

    return 0;
}