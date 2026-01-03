// To compile: g++ -o rvo rvo.cpp
// To run:     ./rvo

#include <iostream> // 提供标准输入输出功能，用于打印对象构造/析构等操作信息
#include <utility>  // 提供std::move函数，用于将对象转为右值引用

using namespace std; // 引入std命名空间，简化代码书写

// 全局条件变量，用于控制函数中的分支逻辑，影响返回值优化（RVO）是否生效
bool cond = false;

// 测试类A，用于观察对象的构造、析构、拷贝、移动等操作
class A
{
public:
    // 构造函数：初始化成员变量num_，打印构造信息
    A(int num = 0) : num_{num}
    {
        cout << "Create A(" << num_ << ")\n";
    }

    // 析构函数：对象销毁时调用，打印析构信息
    ~A()
    {
        cout << "Destroy A(" << num_ << ")\n";
    }

    // 拷贝构造函数：用另一个A对象初始化当前对象，打印拷贝信息
    A(const A &rhs) : num_{rhs.num_}
    {
        cout << "Copy A(" << rhs.num_ << ")\n";
    }

    // 拷贝赋值运算符：将另一个A对象的值赋给当前对象，打印赋值信息
    A &operator=(const A &rhs)
    {
        cout << "Copy= A(" << rhs.num_ << ") <- " << num_ << "\n"; // 格式：目标 <- 源
        num_ = rhs.num_;
        return *this;
    }

#ifndef DISABLE_MOVE // 若未定义DISABLE_move宏，则启用移动语义相关函数
    // 移动构造函数：用右值引用的A对象初始化当前对象（资源转移）
    // 标记为noexcept以确保在容器操作中能正确使用移动而非拷贝
    A(A &&rhs) noexcept : num_(rhs.num_)
    {
        cout << "Move A(" << rhs.num_ << ")\n";
        rhs.num_ = -1; // 修改源对象标识，表明其资源已被转移
    }

    // 移动赋值运算符：将右值引用的A对象资源转移给当前对象
    A &operator=(A &&rhs) noexcept
    {
        cout << "Move= A(" << rhs.num_ << ") <- " << num_ << "\n"; // 格式：目标 <- 源
        num_ = rhs.num_;
        rhs.num_ = -1; // 标记源对象资源已转移
        return *this;
    }
#endif

private:
    int num_; // 用于标识对象的成员变量，辅助观察操作对象
};

// 函数1：返回无名临时对象，可触发返回值优化（RVO）
A getA_unnamed_rvo()
{
    return A(); // 返回无名临时对象，编译器可直接在调用处构造，避免拷贝/移动
}

// 函数2：返回具名局部对象，可触发具名返回值优化（NRVO）
A getA_named_rvo()
{
    A a;      // 具名局部对象
    return a; // 编译器可优化为直接在调用处构造a，避免拷贝/移动
}

// 函数3：返回具名对象但用std::move显式转换，抑制RVO/NRVO
A getA_named_rvo_suppressed()
{
    A a;
    return std::move(a); // std::move将a转为右值，强制触发移动构造（抑制优化）
}

// 函数4：存在分支返回不同对象，无法触发RVO
A getA_no_rvo1()
{
    A a(1); // 具名对象a
    if (cond)
    {
        return A(); // 分支1：返回无名临时对象
    }
    else
    {
        return a; // 分支2：返回具名对象a
    }
    // 因返回对象不唯一（可能是临时对象或a），编译器无法优化，需调用移动/拷贝构造
}

// 函数5：存在分支返回两个不同的具名对象，无法触发RVO
A getA_no_rvo2()
{
    A a1(1); // 具名对象a1
    A a2(2); // 具名对象a2
    if (cond)
    {
        return a1; // 分支1：返回a1
    }
    else
    {
        return a2; // 分支2：返回a2
    }
    // 因返回对象不唯一（a1或a2），编译器无法优化，需调用移动/拷贝构造
}

int main()
{
    {
        cout << "*** Calling getA_unnamed_rvo\n";
        A a = getA_unnamed_rvo(); // 预期：仅调用一次构造和析构（RVO生效）
    }
    {
        cout << "*** Calling getA_named_rvo\n";
        A a = getA_named_rvo(); // 预期：仅调用一次构造和析构（NRVO生效）
    }
    {
        cout << "*** Calling getA_named_rvo_suppressed\n";
        A a = getA_named_rvo_suppressed(); // 预期：构造a后触发移动构造（优化被抑制）
    }
    {
        cout << "*** Calling getA_no_rvo1\n";
        A a = getA_no_rvo1(); // 预期：构造a(1)后，因分支返回移动构造（无RVO）
    }
    {
        cout << "*** Calling getA_no_rvo2\n";
        A a = getA_no_rvo2(); // 预期：构造a1(1)和a2(2)后，移动构造返回对象（无RVO）
    }
}