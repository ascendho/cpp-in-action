// To compile: g++ -std=c++20 cxx20_views.cpp -o cxx20_views
// To run:     ./cxx20_views

#include <iostream>                  // 提供标准输入输出流功能（如std::cout）
#include <map>                       // 提供std::map容器（键值对有序存储，默认按键升序）
#include <ranges>                    // 提供C++20范围视图（views）功能，支持对容器的惰性操作
#include <string>                    // 提供std::string字符串类型
#include "../common/ostream_range.h" // 自定义头文件，用于支持范围（range）直接通过operator<<输出

int main()
{
    // 定义并初始化一个std::map，键为int类型，值为std::string类型
    // map默认按键（key）的升序存储元素（此处键1、2、3、4按升序排列）
    std::map<int, std::string> mp{
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}};

    // 使用C++20范围视图（views）对map进行一系列操作，并输出结果
    // 视图操作具有"惰性求值"特性：不会创建新容器，仅在输出时实时计算，效率高
    std::cout << (mp | std::views::reverse                  // 反转map的元素顺序（原升序→降序，即4、3、2、1）
                  | std::views::filter([](const auto &pr) { // 筛选元素：保留键（pr.first）为偶数的键值对
                        return pr.first % 2 == 0;
                    }) |
                  std::views::values // 提取键值对中的"值"（value）部分，丢弃键（key）
                  )
              << '\n'; // 输出结果：经过反转、筛选、提取值后，得到 {"four", "two"}（因4和2是偶数键）
}