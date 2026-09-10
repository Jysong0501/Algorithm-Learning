#include <iostream>
#include <vector>
#include <unordered_map>
#include <windows.h> // 解决 Windows 终端中文乱码

using namespace std;

int main() {
    // 1. 解决 Windows 控制台中文乱码
    SetConsoleOutputCP(CP_UTF8);
    // 2. 优化输入输出流速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=================== 1. 一维前缀和基础 (1D Prefix Sum) ===================\n";
    /*
     * 【为什么要用前缀和？】
     * 假设数组长度为 N，有 Q 次区间和查询（每次问下标 [L, R] 之间的元素总和）：
     * - 暴力遍历法：每次查询耗时 O(N)，总时间复杂度 O(Q * N)。当 N, Q = 10^5 时会执行 10^10 次，必然超时！
     * - 前缀和算法：O(N) 预处理，单次查询 O(1) 极速出结果，总时间复杂度 O(N + Q)。
     *
     * ⭐️【黄金法则：下标建议从 1 开始 (1-indexed)】
     * 让 prefix[0] = 0。这样当查询从第 1 个元素开始的区间 [1, R] 时，
     * 公式 prefix[R] - prefix[L - 1] -> prefix[R] - prefix[0] 依然成立，完全不需要写 if(L == 0) 的特判！
     *
     * 【核心公式】：
     * 1. 预处理：prefix[i] = prefix[i - 1] + a[i]
     * 2. 查询 [L, R] 区间和：sum(L, R) = prefix[R] - prefix[L - 1]
     */

    // 原数组（为方便理解，我们在第 0 位补 0，有效数据从下标 1 开始：1 到 5）
    // 原序列: [1, 3, 7, 5, 2]
    vector<int> a = {0, 1, 3, 7, 5, 2};
    int n = a.size() - 1;

    // 构建前缀和数组（注意数据范围：竞赛中如果总和可能超过 2*10^9，prefix 必须开 long long！）
    vector<long long> prefix(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        prefix[i] = prefix[i - 1] + a[i];
    }

    // 打印前缀和数组
    cout << "原数组 a:      ";
    for (int i = 1; i <= n; ++i) cout << a[i] << " ";
    cout << "\n前缀和 prefix: ";
    for (int i = 1; i <= n; ++i) cout << prefix[i] << " ";
    cout << "\n";

    // 测试区间查询：求区间 [2, 4] 的和（即 a[2] + a[3] + a[4] = 3 + 7 + 5 = 15）
    int L = 2, R = 4;
    long long range_sum = prefix[R] - prefix[L - 1]; // prefix[4] - prefix[1] = 16 - 1 = 15
    cout << "区间 [" << L << ", " << R << "] 的和为: " << range_sum << "\n";


    cout << "\n=================== 2. 二维前缀和 (2D Matrix Prefix Sum) ===================\n";
    /*
     * 【问题场景】：
     * 给定一个 N x M 的矩阵，多次查询某个子矩形区域 [(x1, y1) 到 (x2, y2)] 内部所有数字的总和。
     *
     * 【几何容斥原理图解】：
     *
     *   (0,0) ----------- (0, y2)
     *     |                  |
     *     |    (x1, y1) ---- |
     *     |       |  目标区域 |
     *   (x2,0) -- | -------- (x2, y2)
     *
     * 1. 预处理二维前缀和矩阵 S[i][j]（表示以 (1,1) 为左上角、(i,j) 为右下角的大矩形总和）：
     *    S[i][j] = S[i-1][j] + S[i][j-1] - S[i-1][j-1] + matrix[i][j]
     *    （上面一块 + 左边一块 - 重叠多加的左上一块 + 当前格子的值）
     *
     * 2. 查询子矩形 [(x1, y1), (x2, y2)] 的总和：
     *    ans = S[x2][y2] - S[x1-1][y2] - S[x2][y1-1] + S[x1-1][y1-1]
     *    （大矩形 - 上方多余 - 左方多余 + 重复减去的左上角）
     */

    int rows = 3, cols = 4;
    // 模拟 3x4 矩阵（1-indexed，第 0 行第 0 列作为占位边框 0）
    vector<vector<int>> mat = {
        {0, 0, 0, 0, 0},
        {0, 1, 2, 3, 4},
        {0, 5, 6, 7, 8},
        {0, 9, 1, 2, 3}
    };

    // 构建二维前缀和 S
    vector<vector<long long>> S(rows + 1, vector<long long>(cols + 1, 0));
    for (int i = 1; i <= rows; ++i) {
        for (int j = 1; j <= cols; ++j) {
            S[i][j] = S[i - 1][j] + S[i][j - 1] - S[i - 1][j - 1] + mat[i][j];
        }
    }

    // 查询子矩形：左上角 (2, 2) 到 右下角 (3, 3)
    // 对应元素：mat[2][2]=6, mat[2][3]=7, mat[3][2]=1, mat[3][3]=2 -> 和为 16
    int x1 = 2, y1 = 2, x2 = 3, y2 = 3;
    long long sub_matrix_sum = S[x2][y2] - S[x1 - 1][y2] - S[x2][y1 - 1] + S[x1 - 1][y1 - 1];
    cout << "子矩形 [(" << x1 << "," << y1 << ") -> (" << x2 << "," << y2 << ")] 的和为: " 
         << sub_matrix_sum << "\n";


    cout << "\n=================== 3. 经典面试与算法题：前缀和 + 哈希表 ===================\n";
    /*
     * 【题目】：找到连续子数组和等于 K 的子数组总个数（LeetCode 560 题）
     * 【数学转化】：
     *   区间 [i, j] 的和为 K  <=>  prefix[j] - prefix[i - 1] == K
     *   移项变形得：prefix[i - 1] == prefix[j] - K
     *
     * 也就是说，当我们遍历到当前前缀和 prefix[j] 时，只要看之前出现过多少次等于 (prefix[j] - K) 的前缀和即可！
     * 用 unordered_map 统计前缀和出现次数，复杂度仅需 O(N)！
     */
    vector<int> nums = {1, 1, 1}; // 寻找和等于 2 的子数组
    int k = 2;

    unordered_map<int, int> prefix_cnt;
    prefix_cnt[0] = 1; // 关键边界：前缀和为 0 默认出现过 1 次（代表空前缀）

    int current_sum = 0;
    int count = 0;

    for (int num : nums) {
        current_sum += num;
        // 如果 (current_sum - k) 曾经出现过，说明存在区间和等于 k
        if (prefix_cnt.count(current_sum - k)) {
            count += prefix_cnt[current_sum - k];
        }
        // 记录当前前缀和的频次
        prefix_cnt[current_sum]++;
    }
    cout << "数组 [1, 1, 1] 中子数组和为 " << k << " 的个数: " << count << "\n";


    cout << "\n=================== 4. 扩展：前缀和的逆运算——差分 (Difference) ===================\n";
    /*
     * 【前缀和 vs 差分】：
     * - 前缀和：针对【频繁区间求和查询】，每次查询 O(1)。
     * - 差分数组：针对【频繁区间修改（例如给 [L, R] 每个数都加上 V）】，每次修改仅需 O(1)！
     * 
     * 修改完毕后，对差分数组求一次【前缀和】，就能完美还原出最终的数组。
     */
    vector<int> diff(n + 2, 0); // 差分数组

    // 模拟操作：给区间 [2, 4] 内的所有数全部加上 10
    int diff_L = 2, diff_R = 4, val = 10;
    diff[diff_L] += val;     // L 处加上 val
    diff[diff_R + 1] -= val; // R + 1 处减去 val

    // 对差分数组做一次前缀和，还原修改后的变化量
    cout << "区间修改 [2, 4] 增加 10 后的各位置增量:\n";
    int delta = 0;
    for (int i = 1; i <= n; ++i) {
        delta += diff[i];
        cout << "下标 " << i << " 增加的值为: " << delta << "\n";
    }

    return 0;
}