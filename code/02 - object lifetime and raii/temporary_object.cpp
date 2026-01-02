// To compile: g++ -o temporary_object temporary_object.cpp
// To run:     ./temporary_object

#include <stdio.h>

// 定义形状基类Shape，作为所有具体形状类的父类
class Shape
{
public:
    // 虚析构函数：确保子类对象通过基类指针销毁时能正确调用子类析构函数
    virtual ~Shape() {}
};

// 定义Circle类，继承自Shape，代表圆形
class Circle : public Shape
{
public:
    // 构造函数：对象创建时调用，输出提示信息（用于观察对象创建）
    Circle() { puts("Circle()"); }
    // 析构函数：对象销毁时调用，输出提示信息（用于观察对象销毁）
    ~Circle() { puts("~Circle()"); }
};

// 定义Triangle类，继承自Shape，代表三角形
class Triangle : public Shape
{
public:
    // 构造函数：对象创建时调用，输出提示信息
    Triangle() { puts("Triangle()"); }
    // 析构函数：对象销毁时调用，输出提示信息
    ~Triangle() { puts("~Triangle()"); }
};

// 定义Result类，代表处理形状后的结果
class Result
{
public:
    // 构造函数：对象创建时调用，输出提示信息
    Result() { puts("Result()"); }
    // 析构函数：对象销毁时调用，输出提示信息
    ~Result() { puts("~Result()"); }
};

// 处理形状的函数：接收两个Shape类型的常量引用（可接收子类对象，体现多态）
// 返回一个Result对象，用于演示函数返回临时对象的生命周期
Result process_shape(const Shape &shape1, const Shape &shape2)
{
    puts("process_shape()"); // 输出函数执行标识
    return Result();         // 返回一个临时的Result对象（会触发Result构造函数）
}

int main()
{
    puts("main()"); // 标识main函数开始执行

    // 调用process_shape函数，传入两个临时对象：
    // 1. 临时Circle对象（会触发Circle构造函数）
    // 2. 临时Triangle对象（会触发Triangle构造函数）
    // 注意：临时对象的生命周期通常持续到包含它的完整表达式结束
    process_shape(Circle(), Triangle());

    puts("something else"); // 标识process_shape调用后继续执行的代码

    // 程序执行流程中对象的创建与销毁顺序：
    // 1. 执行main()，输出"main()"
    // 2. 调用process_shape前，先创建临时Circle和Triangle对象：
    //    - 输出"Circle()"、"Triangle()"
    // 3. 进入process_shape函数，输出"process_shape()"
    // 4. 函数内创建并返回临时Result对象，输出"Result()"
    // 5. 函数返回后，临时Result对象生命周期结束，输出"~Result()"
    // 6. process_shape调用表达式结束，临时Circle和Triangle对象销毁：
    //    - 输出"~Triangle()"、"~Circle()"（析构顺序与构造相反）
    // 7. 输出"something else"，程序结束
    return 0;
}

/*
 * 临时对象（如代码中的 Circle() 和 Triangle()）的析构顺序与构造顺序相反，
 * 核心原因是遵循 “栈的先进后出（LIFO）特性”。
 * “构造与析构相反” 的规则本质是为了避免资源依赖问题。
 * 例如：如果对象 A 依赖对象 B 的资源（A 构造时使用了 B 的资源），
 * 那么 A 必须在 B 之后构造，且在 B 之前析构（否则 A 析构时 B 的资源可能已被释放，导致错误）。
 * 栈的 LIFO 特性天然满足了这种依赖顺序。
 *
 */