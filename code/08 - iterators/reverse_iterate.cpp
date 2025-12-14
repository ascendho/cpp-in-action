// To compile: g++ reverse_iterate.cpp -o reverse_iterate
// To run:    ./reverse_iterate

// 程序功能: 演示两种反向遍历容器的方法（普通迭代器反向遍历 + 反向迭代器遍历）
// 适用容器: 支持迭代器的STL容器（如vector、list、deque等）

// 引入输入输出流头文件，用于cout输出内容到控制台
#include <iostream>
// 引入vector容器头文件，用于使用动态数组容器std::vector
#include <vector>

// 使用std命名空间，避免重复书写std::前缀（简化代码）
using namespace std;

/**
 * @brief 模板函数：反向输出容器中的元素
 * @tparam C 模板参数，代表任意支持迭代器的STL容器类型（如vector、list等）
 * @param c 常量引用，传入需要反向输出的容器（const保证容器不被修改，引用避免拷贝）
 */
template <typename C>
void reverseOutput(const C &c)
{
    // 方法1：使用普通迭代器反向遍历
    // 1. 初始化迭代器it为容器末尾（end()指向最后一个元素的下一个位置，不指向有效元素）
    // 2. 循环条件：it不等于容器起始位置（begin()指向第一个元素）
    // 3. 先自减迭代器（让it指向当前最后一个有效元素），再解引用输出
    for (auto it = c.end(); it != c.begin();)
    {
        --it;               // 迭代器自减，指向前一个元素（从end()移动到最后一个有效元素）
        cout << *it << ' '; // 解引用迭代器，输出当前指向的元素，空格分隔
    }
    cout << '\n';

    // 方法2：使用反向迭代器（rbegin()/rend()）直接遍历
    // 1. rbegin()指向容器最后一个元素（反向迭代器的起始位置）
    // 2. rend()指向容器第一个元素的前一个位置（反向迭代器的结束位置）
    // 3. 反向迭代器的++操作等价于普通迭代器的--操作，符合直觉的"正向"遍历反向序列
    for (auto it = c.rbegin(); it != c.rend(); ++it)
    {
        cout << *it << ' '; // 解引用反向迭代器，输出元素，空格分隔
    }
    cout << '\n'; // 换行，结束输出
}

int main()
{
    vector<int> v{1, 2, 3, 4, 5};
    reverseOutput(v);

    return 0;
}