#include <functional> // 引入std::function，用于存储函数对象
#include <iostream>   // 引入输入输出流，用于控制台输出
#include <map>        // 引入std::map，用于存储运算符与对应运算函数的映射

using namespace std; // 使用std命名空间，简化代码书写

int main()
{
    // 定义四个计数器，分别记录加、减、乘、除四种运算的调用次数
    int count_plus{};       // 加法运算计数器
    int count_minus{};      // 减法运算计数器
    int count_multiplies{}; // 乘法运算计数器
    int count_divides{};    // 除法运算计数器

    // 定义一个map容器，键为运算符字符串，值为接收两个int参数并返回int的函数对象
    // 用于存储运算符与对应运算逻辑的映射，并在运算时更新对应计数器
    map<string, function<int(int, int)>> ops{
        {"+",                          // 加法运算符
         [&count_plus](int x, int y) { // 捕获加法计数器的lambda表达式
             ++count_plus;             // 调用加法时，计数器加1
             return x + y;             // 执行加法运算并返回结果
         }},
        {"-",                           // 减法运算符
         [&count_minus](int x, int y) { // 捕获减法计数器的lambda表达式
             ++count_minus;             // 调用减法时，计数器加1
             return x - y;              // 执行减法运算并返回结果
         }},
        {"*",                                // 乘法运算符
         [&count_multiplies](int x, int y) { // 捕获乘法计数器的lambda表达式
             ++count_multiplies;             // 调用乘法时，计数器加1
             return x * y;                   // 执行乘法运算并返回结果
         }},
        {"/",                             // 除法运算符
         [&count_divides](int x, int y) { // 捕获除法计数器的lambda表达式
             ++count_divides;             // 调用除法时，计数器加1
             return x / y;                // 执行除法运算并返回结果
         }},
    };

    // 执行运算：先计算5*8（乘法），再将结果与2相加（加法），并输出最终结果
    // 此时会触发乘法计数器和加法计数器各加1
    cout << ops.at("+")(ops.at("*")(5, 8), 2) << '\n';
    // 输出乘法运算的调用次数（预期为1）
    cout << count_multiplies << '\n';
    // 输出加法运算的调用次数（预期为1）
    cout << count_plus << '\n';
}