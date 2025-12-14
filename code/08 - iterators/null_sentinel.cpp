// To compile: g++ null_sentinel.cpp -o null_sentinel
// To run:     ./null_sentinel

// 程序功能：演示「哨兵迭代器（Sentinel Iterator）」的自定义实现，
//           实现对C风格字符串（以'\0'结尾）的灵活遍历，支持：
//           1. 普通for循环 + 自定义哨兵
//           2. 范围for循环（Range-based for）
//           3. C++20 std::ranges::for_each（条件编译兼容）
#include <iostream> // 引入输入输出流头文件，提供std::cout用于控制台输出

// 条件编译：仅当编译器支持C++20及以上标准时，引入algorithm头文件
// std::ranges::for_each是C++20新增的范围遍历算法
#if __cplusplus >= 202002L
#include <algorithm> // 提供std::ranges::for_each算法
#endif

using std::cout; // 引入std::cout，简化代码中cout的使用（避免重复写std::）

/**
 * @brief 自定义哨兵类型（Sentinel Type）
 * 哨兵不是传统迭代器，而是用于标记「迭代终止条件」的特殊类型，
 * 此处用于判断C字符串是否遍历到结束符'\0'（即数值0）
 */
struct null_sentinel
{
    // 空结构体：仅作为类型标记，无成员变量/函数
};

/**
 * @brief 重载迭代器与null_sentinel的==运算符（迭代器在前，哨兵在后）
 * @tparam I 迭代器类型（此处实际为const char*，兼容任意解引用返回可比较0的迭代器）
 * @param i 待判断的迭代器（指向C字符串的当前字符）
 * @param null_sentinel 哨兵实例（仅作类型标记）
 * @return bool 迭代器指向的字符为'\0'（数值0）时返回true，代表迭代终止
 */
template <typename I>
bool operator==(I i, null_sentinel)
{
    return *i == 0; // 解引用迭代器，判断是否为C字符串结束符'\0'
}

/**
 * @brief 重载null_sentinel与迭代器的==运算符（哨兵在前，迭代器在后）
 * 保证比较的对称性（sentinel == iter 和 iter == sentinel 等价）
 * @tparam I 迭代器类型（此处为const char*）
 * @param null_sentinel 哨兵实例
 * @param i 待判断的迭代器
 * @return bool 迭代器指向'\0'时返回true
 */
template <typename I>
bool operator==(null_sentinel, I i)
{
    return *i == 0; // 与上一个重载逻辑一致，保证对称性
}

/**
 * @brief 重载迭代器与null_sentinel的!=运算符（迭代器在前）
 * 为==的取反，满足循环条件中!=的使用需求
 * @tparam I 迭代器类型
 * @param i 迭代器
 * @param null_sentinel 哨兵实例
 * @return bool 迭代器指向的字符非'\0'时返回true，代表继续迭代
 */
template <typename I>
bool operator!=(I i, null_sentinel)
{
    return *i != 0;
}

/**
 * @brief 重载null_sentinel与迭代器的!=运算符（哨兵在前）
 * 保证比较对称性，满足循环条件的灵活使用
 * @tparam I 迭代器类型
 * @param null_sentinel 哨兵实例
 * @param i 迭代器
 * @return bool 迭代器指向的字符非'\0'时返回true
 */
template <typename I>
bool operator!=(null_sentinel, I i)
{
    return *i != 0;
}

/**
 * @brief C字符串读取器类，适配「范围for循环」的接口
 * 实现begin()返回字符串起始指针，end()返回null_sentinel，
 * 让范围for循环能自动遍历C字符串到'\0'为止
 */
class c_string_reader
{
public:
    /**
     * @brief 构造函数：初始化C字符串指针
     * @param s 待遍历的C风格字符串（const char*类型，以'\0'结尾）
     */
    c_string_reader(const char *s) : ptr_(s) {}

    /**
     * @brief 范围for循环所需：返回迭代起始位置
     * @return const char* C字符串的起始指针（迭代器起始位置）
     */
    const char *begin() const { return ptr_; }

    /**
     * @brief 范围for循环所需：返回迭代终止哨兵
     * @return null_sentinel 自定义哨兵实例，标记迭代终止条件（*ptr == '\0'）
     */
    null_sentinel end() const { return {}; }

private:
    const char *ptr_; // 存储C字符串的起始地址（私有成员，仅内部访问）
};

// 主函数：程序入口，演示三种C字符串遍历方式
int main()
{
    // 定义C风格字符串，末尾隐式包含'\0'结束符（字符串字面量的默认特性）
    const char *msg = "Hello world!\n";

    // 方式1：普通for循环 + 自定义null_sentinel作为终止条件
    // 迭代器：const char* ptr（指向当前字符）
    // 终止条件：ptr != null_sentinel{}（即*ptr != '\0'）
    // 迭代操作：++ptr（指针自增，指向下一个字符）
    for (auto ptr = msg; ptr != null_sentinel{}; ++ptr)
    {
        cout << *ptr; // 输出当前指向的字符
    }

    // 方式2：范围for循环 + c_string_reader适配类
    // 范围for会自动调用c_string_reader的begin()和end()，
    // 结合重载的==/!=运算符，遍历到*ptr == '\0'时终止
    for (char ch : c_string_reader(msg))
    {
        cout << ch; // 输出遍历到的每个字符
    }

    // 方式3：C++20特性 - std::ranges::for_each + 自定义哨兵
    // 仅当编译器支持C++20时编译此段代码
#if __cplusplus >= 202002L
    // std::ranges::for_each支持「迭代器-哨兵」对作为范围，
    // 第三个参数是lambda表达式，处理每个遍历到的字符
    std::ranges::for_each(msg, null_sentinel{},
                          [](char ch)
                          { cout << ch; }); // lambda表达式：输出字符
#endif

    return 0; // 程序正常退出，返回0表示执行成功
}