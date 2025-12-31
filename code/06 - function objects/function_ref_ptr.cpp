#include <iostream> // 引入输入输出流库，用于控制台输出（如std::cout）

using namespace std; // 使用std命名空间，简化标准库组件的调用

// 定义一个简单的函数：接收一个int类型参数x，返回x加2的结果
int add2(int x)
{
    return x + 2;
};

// 模板函数test1：参数为任意类型T的函数对象fn
// 功能：调用fn并传入参数2，返回调用结果
template <typename T>
auto test1(T fn) // T可推导为函数指针类型（因为传入函数名时会隐式转换为指针）
{
    return fn(2); // 调用函数对象fn，传入2（等价于add2(2)）
}

// 模板函数test2：参数为任意类型T的函数对象的引用fn
// 功能：通过引用调用fn并传入参数2，返回调用结果
template <typename T>
auto test2(T &fn) // T可推导为函数类型，fn为函数的引用
{
    return fn(2); // 通过引用调用函数，传入2（等价于add2(2)）
}

// 模板函数test3：参数为任意类型T的函数对象的指针fn
// 功能：解引用指针后调用函数，并传入参数2，返回调用结果
template <typename T>
auto test3(T *fn) // T*为函数指针类型（显式接收函数指针）
{
    return (*fn)(2); // 解引用指针得到函数，再调用并传入2（等价于add2(2)）
}

int main()
{
    // 调用test1，传入函数名add2（隐式转换为函数指针）
    // test1内部调用add2(2)，结果为4，输出4
    cout << test1(add2) << '\n';

    // 调用test2，传入函数名add2（作为函数引用传递）
    // test2内部通过引用调用add2(2)，结果为4，输出4
    cout << test2(add2) << '\n';

    // 调用test3，传入函数名add2（隐式转换为函数指针）
    // test3内部解引用指针后调用add2(2)，结果为4，输出4
    cout << test3(add2) << '\n';
}