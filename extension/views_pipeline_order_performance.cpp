// To compile: g++ -std=c++20 views_pipeline_order_performance.cpp -o pipeline_perf
// To run:     ./pipeline_perf

#include <iostream>
#include <map>       // 提供std::map容器
#include <ranges>    // 提供C++20范围视图（views）
#include <string>    // 提供std::string
#include <algorithm> // 提供std::ranges::for_each（触发视图遍历）

// 简化视图的命名空间（方便写views::xxx）
using namespace std::views;

int main()
{
    // 初始化原map：键为int，值为string，共4个元素
    std::map<int, std::string> mp{
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};

    // ========== 情况1：合理顺序：reverse → filter → values ==========
    int filter_count1 = 0; // 统计filter1的求值次数
    // 定义带计数器的filter：检查key是否为偶数，每次调用计数器+1
    auto filter1 = [&filter_count1](const auto &key_value_pair)
    {
        ++filter_count1;                      // 每次进入filter逻辑，计数器自增
        return key_value_pair.first % 2 == 0; // 筛选key为偶数的元素
    };

    // 管道顺序：先反转map → 再筛选 → 最后提取值
    auto result1 = mp | reverse | filter(filter1) | values;
    // 触发视图遍历（视图是惰性的，必须遍历才会执行filter逻辑）
    std::ranges::for_each(result1, [](const auto &) {});

    std::cout << "情况1（reverse → filter）：filter求值次数 = " << filter_count1 << "\n";
    // 结果：4次（只遍历反转后的4个元素，筛选1次）

    // ========== 情况2：不合理顺序：filter → reverse → values ==========
    int filter_count2 = 0; // 统计filter2的求值次数
    auto filter2 = [&filter_count2](const auto &key_value_pair)
    {
        ++filter_count2;
        return key_value_pair.first % 2 == 0;
    };

    // 管道顺序：先筛选 → 再反转 → 最后提取值
    auto result2 = mp | filter(filter2) | reverse | values;
    // 触发视图遍历
    std::ranges::for_each(result2, [](const auto &) {});

    std::cout << "情况2（filter → reverse）：filter求值次数 = " << filter_count2 << "\n";
    // 结果：8次（reverse是双向范围，需2次遍历filter结果，每次触发4次筛选）

    return 0;
}

/*
 * 核心原因：
 * 1. reverse是“双向范围”，需要先正向遍历filter结果找“尾元素”，再反向遍历生成序列；
 * 2. filter是“惰性无缓存”的，每次遍历filter结果都会重新执行筛选逻辑；
 * 3. 因此情况2中，filter被触发2次（每次4个元素），总次数=4×2=8。
 */