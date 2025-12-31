#include <functional> // 包含标准库中与函数对象相关的工具：std::bind、std::function、std::plus等
#include <iostream>   // 包含输入输出流，用于控制台输出
#include <map>        // 包含std::map容器，用于存储键值对映射
#include <string>     // 包含字符串处理相关功能

using namespace std; // 使用std命名空间，简化代码中标准库组件的调用

// 自定义函数对象结构体：实现"加n"的操作
struct Adder
{
    Adder(int n) : n_(n) {} // 构造函数，初始化要添加的数值n_
    // 重载函数调用运算符()，使Adder对象可像函数一样被调用
    // 功能：接收参数x，返回x与n_的和
    int operator()(int x) const
    {
        return x + n_;
    }

private:
    int n_; // 存储要添加的固定数值
};

int main()
{
    // 示例1：使用自定义函数对象Adder
    {
        Adder add2(2); // 创建Adder对象add2，固定添加2
        // 调用函数对象：等价于add2.operator()(5)，计算5+2
        cout << "Manual function object: " << add2(5) << '\n'; // 输出：7
    }

    // 示例2：使用std::bind2nd（C++17前的旧接口，已弃用）
    // 条件编译：仅在C++标准低于C++17时启用，且处理GCC的弃用警告
#if __cplusplus < 201703L
#if defined(__GNUC__)
#pragma GCC diagnostic push                                // 保存当前编译器诊断状态
#pragma GCC diagnostic ignored "-Wdeprecated-declarations" // 忽略弃用警告
#endif
    {
        // std::bind2nd：将二元函数std::plus<int>()（加法）的第二个参数绑定为2
        // 生成一个一元函数对象，功能：接收x，返回x+2
        auto add2 = bind2nd(plus<int>(), 2);
        cout << "bind2nd: " << add2(5) << '\n'; // 输出：7
    }
#if defined(__GNUC__)
#pragma GCC diagnostic pop // 恢复编译器诊断状态
#endif
#endif

    // 示例3：使用std::bind（更灵活的函数绑定工具）
    {
        using namespace std::placeholders; // 引入占位符_1, _2等
        // std::bind：将二元函数std::plus<int>()的第二个参数绑定为2
        // _1表示第一个参数为占位符（调用时传入），生成一元函数对象x -> x+2
        // NOLINT注释：忽略工具对使用bind的警告（建议现代代码用lambda）
        auto add2 = bind(plus<int>(), _1, 2);
        cout << "bind: " << add2(5) << '\n'; // 输出：7
    }

    // 示例4：使用lambda表达式（C++11起的匿名函数，简洁的函数对象）
    {
        // 定义lambda表达式：接收int x，返回x+2
        auto add2 = [](int x)
        { return x + 2; };
        cout << "Lambda: " << add2(5) << '\n'; // 输出：7
    }

    // 示例5：柯里化（Currying）：将多参数函数转换为嵌套的单参数函数
    {
        // 外层lambda：接收n，返回一个内层lambda
        // 内层lambda：接收x，返回x + n（捕获外层的n）
        auto adder = [](int n)
        { return [n](int x)
          { return x + n; }; };
        // 先调用adder(2)得到一个"加2"的函数，再传入5计算
        cout << "Currying: " << adder(2)(5) << '\n'; // 输出：7
    }

    // 示例6：使用std::function存储不同函数对象，结合map实现"运算符-运算"映射
    {
        // 定义map：键为运算符字符串，值为std::function（可存储函数/函数对象/lambda等）
        // 存储加减乘除四种运算的lambda函数
        map<string, function<int(int, int)>> op_dict{
            {"+", [](int x, int y)
             { return x + y; }}, // 加法
            {"-", [](int x, int y)
             { return x - y; }}, // 减法
            {"*", [](int x, int y)
             { return x * y; }}, // 乘法
            {"/", [](int x, int y)
             { return x / y; }}, // 除法
        };
        // 通过键"+"获取加法函数，传入1和6计算1+6
        cout << "std::function: " << op_dict.at("+")(1, 6) << '\n'; // 输出：7
    }
}