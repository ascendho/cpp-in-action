// To compile: g++ string_concat.cpp -o string_concat
// To run:     ./string_concat

#include <iostream>
#include <string>

using namespace std;

/**
 * @brief 方式1：分步拼接字符串（salute1）
 * @param name 传入的用户名
 * @return 拼接完成的问候字符串
 *
 * 实现逻辑：
 * 1. 先创建一个持久的string对象msg，初始化为开头的"Hi, "
 * 2. 通过`+=`运算符**分步追加内容**（先加name，再加后续字符串）
 * 3. 最终返回这个拼接好的msg对象
 *
 * 底层特点：
 * - 所有操作基于同一个string对象，内存扩容可能分多次进行（若内容超过当前容量）
 * - 过程式风格，步骤清晰
 */
string salute1(const string &name)
{
    string msg = "Hi, ";           // 初始化基础字符串
    msg += name;                   // 追加用户名（第一次修改msg）
    msg += ", how are you today?"; // 追加结尾字符串（第二次修改msg）
    return msg;
}

/**
 * @brief 方式2：单次表达式拼接字符串（salute2）
 * @param name 传入的用户名
 * @return 拼接完成的问候字符串
 *
 * 实现逻辑：
 * 1. 先通过`string{"Hi, "}`将C风格字符串转为string对象（关键！否则两个const char*不能直接用+）
 * 2. 通过`+`运算符**连续拼接**：先拼接name，再拼接结尾字符串
 * 3. 整个表达式的结果是一个临时string对象，直接返回
 *
 * 底层特点：
 * - 逻辑上基于临时string对象的连续运算（每次+都会生成新临时对象，原临时对象销毁）
 * - 现代编译器会优化这种连续拼接（提前计算总长度，减少内存分配次数）
 * - 声明式风格，代码更简洁
 */
string salute2(const string &name)
{
    // 注意：若写成 `"Hi, " + name` 会编译失败（C++不支持两个const char*直接用+）
    // string{"Hi, "} 先把第一个字符串字面量转为 string 对象，后续的 + 是基于 string 重载的 operator+（支持 string 与 const char* 相加），
    // 因此语法合法。
    return string{"Hi, "} + name + ", how are you today?";
}

int main()
{
    const string username = "Alice"; // 定义测试用的用户名

    // 调用两种方式的函数，获取结果
    string result1 = salute1(username);
    string result2 = salute2(username);

    // 打印结果，验证功能一致性
    cout << "salute1的结果：" << result1 << endl;
    cout << "salute2的结果：" << result2 << endl;
    cout << "两个结果是否相同：" << (result1 == result2 ? "是" : "否") << endl;

    /**
     * 区别总结：
     * 1. 拼接方式：
     *    - salute1：分步`+=`（同一string对象上追加）
     *    - salute2：单次`+`（临时string对象的连续运算）
     * 2. 内存行为：
     *    - salute1：同一对象可能多次扩容
     *    - salute2：临时对象运算（编译器可能优化为少次扩容）
     * 3. 代码风格：
     *    - salute1：过程式（多步骤）
     *    - salute2：声明式（简洁一行）
     *
     * 联系总结：
     * - 最终功能完全一致：都生成"Hi, [name], how are you today?"格式的字符串
     * - 都依赖C++ string的运算符重载（`+=`和`+`）实现拼接
     */

    return 0;
}