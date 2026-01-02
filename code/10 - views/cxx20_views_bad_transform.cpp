// To compile: g++ -std=c++20 cxx20_views_bad_transform.cpp -o bad_transform
// To run:     ./bad_transform

#include <iostream>                  // 提供标准输入输出功能（如std::cout）
#include <map>                       // 提供std::map容器（键值对有序存储，默认按key升序）
#include <ranges>                    // 提供C++20范围视图（views）功能，支持惰性操作
#include <string>                    // 提供std::string字符串类型
#include "../common/ostream_range.h" // 自定义头文件，支持直接通过operator<<输出范围（range）内容

int main()
{
    // 定义并初始化一个std::map，键为int类型，值为std::string类型
    // 存储的键值对为：{1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}
    // map默认按key升序排列，因此内部顺序为1→2→3→4
    std::map<int, std::string> mp{
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};

    // 定义计数器，用于统计transform操作被执行的次数
    int tf_count{}; // 初始化为0

    // 使用C++20范围视图对map进行链式操作，并输出结果
    // 视图特性：惰性求值（仅在遍历/输出时执行实际操作，而非定义时）
    std::cout << (mp | std::views::transform([&tf_count](const auto &pr) { // 第一个视图：转换
                      ++tf_count;                                          // 每次执行转换时，计数器自增（记录转换次数）
                      return pr.first;                                     // 提取键值对中的key（int类型）
                  }) |
                  std::views::filter([](int num) { // 第二个视图：筛选
                      return num % 2 == 0;         // 保留偶数key
                  }))
              << '\n'; // 输出结果：经过转换（提取key）和筛选（保留偶数）后，得到 2 4

    // 输出transform操作的实际执行次数
    // 由于惰性求值，transform仅对map中所有元素执行（共6次），而非仅对筛选后的元素
    std::cout << tf_count << " transformations are made\n"; // 输出：6 transformations are made
}