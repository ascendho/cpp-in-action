// To compile: g++ -std=c++20 transparent_hash.cpp -o transparent_hash
// To run:     ./transparent_hash


#include <cstddef>       // 提供std::size_t类型（用于哈希值的返回类型）
#include <functional>    // 提供std::equal_to（透明比较器）和std::hash（哈希函数）
#include <iostream>      // 提供标准输入输出功能（如std::cout）
#include <string>        // 提供std::string字符串类型
#include <string_view>   // 提供std::string_view（轻量字符串视图，避免临时字符串构造）
#include <unordered_set> // 提供std::unordered_set（无序哈希集合）

// 自定义哈希函数结构体，支持透明哈希（异质查找）
struct MyStrHash
{
    // 声明透明特性：通过定义is_transparent类型，告诉容器该哈希函数支持异质查找
    // 允许哈希函数接收与容器键类型（此处为std::string）不同但可转换的类型（如std::string_view、const char*）
    using is_transparent = void;

    // 重载哈希函数调用运算符，接收std::string_view作为参数
    // std::string_view可直接引用字符串字面量或std::string，无需构造临时std::string，提高效率
    std::size_t operator()(std::string_view str) const noexcept
    {
        // 复用std::hash<std::string_view>的哈希实现，计算字符串视图的哈希值
        return std::hash<std::string_view>{}(str);
    }
};

int main()
{
    using namespace std; // 引入std命名空间，简化代码
    cout << boolalpha;   // 设置cout输出bool值为"true"/"false"，而非1/0

    // 定义无序哈希集合：
    // - 键类型为std::string（存储的元素类型）
    // - 哈希函数使用自定义的MyStrHash（支持透明哈希）
    // - 比较器使用std::equal_to<>（透明比较器，支持不同类型的相等性比较）
    unordered_set<string, MyStrHash, equal_to<>> s{
        "one", "two", "three" // 初始化集合，包含三个字符串
    };

    // 调用contains方法检查集合中是否包含指定元素
    // 透明特性允许直接传入字符串字面量（const char*），无需转换为std::string
    // 避免了临时std::string的构造，提升性能
    cout << s.contains("one") << '\n';  // 输出true（集合包含"one"）
    cout << s.contains("two") << '\n';  // 输出true（集合包含"two"）
    cout << s.contains("tres") << '\n'; // 输出false（集合不包含"tres"）
}