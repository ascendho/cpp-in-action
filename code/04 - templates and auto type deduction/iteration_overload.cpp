// To compile： g++ -o iteration_overload iteration_overload.cpp
// To run：     ./iteration_overload

#include <cstddef> // 包含size_t类型的定义，用于数组大小等场景
#include <vector>  // 包含标准容器vector的定义

using namespace std; // 引入std命名空间，简化标准库组件的使用

// 模板函数foo：针对标准容器的重载版本
// 参数：const C& c - 任意标准容器的const引用（确保元素不被修改）
// 功能：通过容器的const_iterator遍历容器中的元素
template <typename C>
void foo(const C &c)
{
    // 定义容器的常量迭代器，获取容器的起始和结束位置
    // 用end变量缓存c.end()，避免每次循环判断时重复调用（优化性能）
    for (typename C::const_iterator it = c.begin(), end = c.end();
         it != end; ++it)
    { // 遍历容器，直到迭代器到达结束位置
      // 循环体：可对*it（当前元素）进行操作
    }
}

// 模板函数foo：针对C风格固定大小数组的重载版本
// 参数：const T (&a)[N] - 固定大小数组的const引用（避免数组名退化为指针，同时确保元素不被修改）
// 模板参数：T为数组元素类型，N为数组大小（编译期确定）
// 功能：通过指针模拟迭代器，遍历数组中的元素
template <typename T, size_t N>
void foo(const T (&a)[N])
{
    typedef const T *ptr_t; // 定义常量指针类型，作为数组的"迭代器"
    // 用指针遍历数组：起始指针为数组首元素地址，结束指针为尾后地址（a + N）
    for (ptr_t it = a, end = a + N; it != end; ++it)
    {
        // 循环体：可对*it（当前数组元素）进行操作
    }
}

int main()
{
    std::vector<int> v; // 定义一个int类型的vector容器
    v.push_back(1);     // 向容器中添加元素1
    v.push_back(2);     // 向容器中添加元素2
    v.push_back(3);     // 向容器中添加元素3
    foo(v);             // 调用针对容器的foo重载版本，遍历vector

    int a[] = {1, 2, 3}; // 定义一个int类型的C风格数组
    foo(a);              // 调用针对数组的foo重载版本，遍历数组

    return 0; // 程序正常结束
}