// To compile: g++ string_view_lifetime_traps.cpp -o string_view_lifetime_traps
// To run:     ./string_view_lifetime_traps

#include <iostream>
#include <string>      // 提供std::string（管理内存的字符串）
#include <string_view> // 提供std::string_view（不管理内存的字符串视图）

/**
 * @brief 错误示例1：string_view引用“临时std::string”
 * 临时std::string的生命周期仅持续到当前语句结束，后续string_view会悬空
 */
void demo_temporary_string_view()
{
    std::cout << "=== 错误场景1：string_view引用临时string ===\n";

    // 错误点：std::string("临时字符串")是“临时对象”，语句结束后立即销毁
    std::string_view sv = std::string("临时字符串");

    // 此时临时string已被销毁，sv保存的指针指向“已释放的内存”（悬空视图）
    // 访问sv会触发【未定义行为】：可能输出乱码、崩溃，或看似“正常”
    std::cout << "访问悬空的string_view：" << sv << "\n\n";
}

/**
 * @brief 错误示例2：函数返回引用“局部std::string”的string_view
 * 函数内的局部string在函数返回后销毁，返回的string_view会悬空
 */
std::string_view bad_return_string_view()
{
    // 局部std::string：生存周期仅限于当前函数内部
    std::string local_str = "函数内的局部字符串";
    // 返回引用local_str的string_view
    return std::string_view(local_str);
}

int main()
{
    // 演示错误场景1
    demo_temporary_string_view();

    // 演示错误场景2
    std::cout << "=== 错误场景2：返回引用局部string的string_view ===\n";
    // 错误点：函数返回后，local_str已被销毁，sv2是悬空视图
    std::string_view sv2 = bad_return_string_view();
    // 访问sv2同样触发【未定义行为】
    std::cout << "访问函数返回的悬空string_view：" << sv2 << "\n\n";

    return 0;
}

/*
 * 正确用法参考：
 * 确保string_view引用的原字符串，生命周期覆盖string_view的使用周期
 * 例如：
 * std::string valid_str = "有效字符串";
 * std::string_view valid_sv = valid_str; // valid_str的生命周期长于valid_sv
 * std::cout << valid_sv; // 安全
 */