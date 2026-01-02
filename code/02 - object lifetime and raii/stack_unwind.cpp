// To compile: g++ -o stack_unwind stack_unwind.cpp
// To run:     ./stack_unwind

#include <stdio.h>

// 定义一个简单的类Obj，用于观察对象的构造与析构过程
class Obj
{
public:
    // 构造函数：对象创建时被调用，输出提示信息
    Obj() { puts("Obj()"); }
    // 析构函数：对象销毁时被调用，输出提示信息
    ~Obj() { puts("~Obj()"); }
};

// 函数foo：接收一个int参数n，内部创建Obj对象，当n为42时抛出异常
void foo(int n)
{
    Obj obj; // 创建Obj类的局部对象，此时会调用Obj的构造函数输出"Obj()"
    if (n == 42)
    { // 当参数n等于42时
        // 抛出一个字符串类型的异常，触发栈展开过程
        throw "life, the universe and everything";
    }
    // 若n不等于42，函数正常结束，局部对象obj会被销毁（调用析构函数）
}

int main()
{
    try
    {            // 异常捕获的try块：包含可能抛出异常的代码
        foo(41); // 调用foo(41)：n不等于42，不抛异常
                 // 执行后，foo内的局部对象obj会被正常销毁（输出"~Obj()"）
        foo(42); // 调用foo(42)：n等于42，会抛出异常
    }
    // 捕获const char*类型的异常（与抛出的异常类型匹配）
    catch (const char *s)
    {
        puts(s); // 输出异常信息："life, the universe and everything"
    }
    // 程序执行流程：
    // 1. 调用foo(41)：创建Obj（输出"Obj()"），函数正常结束，销毁Obj（输出"~Obj()"）
    // 2. 调用foo(42)：创建Obj（输出"Obj()"），抛出异常，触发栈展开
    // 3. 栈展开过程：foo函数退出，局部对象obj被销毁（输出"~Obj()"），异常被main的catch块捕获
    // 4. catch块输出异常信息，程序正常结束
    return 0;
}

/*
 * 栈展开（stack unwinding）是指当异常被抛出后，
 * 程序沿着函数调用栈回溯并销毁沿途局部对象的过程，
 * 直到找到匹配的异常处理块（catch）。
 * 它的核心作用是保证异常抛出时，
 * 所有在 “抛出点到异常处理块之间” 的局部对象都能被正确销毁（调用析构函数），避免资源泄漏。
 * 这是 C++ 异常处理机制中 “资源安全” 的关键保障。
 *
 */