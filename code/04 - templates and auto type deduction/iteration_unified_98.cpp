// To compile： g++ -o iteration_unified_98 iteration_unified_98.cpp
// To run：     ./iteration_unified_98

#include <cstddef> // 包含size_t类型定义，用于数组大小等编译期常量场景
#include <vector>  // 包含标准容器vector的定义，提供动态数组功能

using namespace std; // 引入std命名空间，简化标准库组件的使用

// 通用traits模板：为支持迭代器的容器提供统一的迭代接口适配
// 作用：通过模板封装容器的迭代器类型及首尾迭代器获取方法，实现接口标准化
template <typename T>
struct traits
{
    // 定义常量迭代器类型，依赖于容器T自身的const_iterator
    typedef typename T::const_iterator const_iterator;

    // 获取容器的起始常量迭代器
    // 参数：const T& c - 容器的const引用，确保容器元素不被修改
    static const_iterator begin(const T &c)
    {
        return c.begin(); // 调用容器自身的begin()方法
    }

    // 获取容器的结束常量迭代器（尾后位置）
    static const_iterator end(const T &c)
    {
        return c.end(); // 调用容器自身的end()方法
    }
};

// traits模板特化：针对C风格固定大小数组的适配
// 目的：让原生数组也能通过traits获得与标准容器一致的迭代接口
template <typename T, size_t N> // T为数组元素类型，N为数组大小（编译期确定）
struct traits<T[N]>
{ // 特化于"元素类型为T、大小为N的数组"类型
    // 数组的常量迭代器用"指向const T的指针"模拟
    typedef const T *const_iterator;

    // 获取数组的起始迭代器（首元素地址）
    // 参数：const T (&a)[N] - 数组的const引用，避免数组名退化为指针，同时保留大小信息
    static const_iterator begin(const T (&a)[N])
    {
        return a; // 数组名自动转换为指向首元素的指针
    }

    // 获取数组的结束迭代器（尾后位置）
    static const_iterator end(const T (&a)[N])
    {
        return a + N; // 首地址 + 数组大小 = 尾后位置（与容器end()语义一致）
    }
};

// 泛型遍历函数foo：通过traits实现对不同类型容器/数组的统一遍历
// 优势：无需为容器和数组分别实现遍历逻辑，体现泛型编程的"一次编写，多处使用"
template <typename T>
void foo(const T &c)
{
    // 通过traits<T>获取对应类型的常量迭代器，统一初始化首尾迭代器
    // 缓存end迭代器，避免循环中重复调用end()（优化性能）
    for (typename traits<T>::const_iterator
             it = traits<T>::begin(c), // 起始位置
         end = traits<T>::end(c);      // 结束位置
         it != end;                    // 遍历终止条件：迭代器到达尾后位置
         ++it)
    { // 移动到下一个元素
      // 循环体可对*it（当前元素）进行操作，此处省略具体实现
    }
}

int main()
{
    // 测试标准容器vector的遍历
    std::vector<int> v; // 定义int类型的vector容器
    v.push_back(1);     // 向容器添加元素1
    v.push_back(2);     // 添加元素2
    v.push_back(3);     // 添加元素3
    foo(v);             // 调用foo函数遍历vector，此时traits匹配通用版本

    // 测试C风格数组的遍历
    int a[] = {1, 2, 3}; // 定义int类型的固定大小数组
    foo(a);              // 调用foo函数遍历数组，此时traits匹配特化版本（T=int, N=3）

    return 0; // 程序正常结束
}