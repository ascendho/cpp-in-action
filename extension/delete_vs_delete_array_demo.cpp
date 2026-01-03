// To compile: g++ delete_vs_delete_array_demo.cpp -o delete_demo
// To run:     ./delete_demo

#include <iostream>
using namespace std;

// 定义一个带析构函数的类，用于观察对象的创建/销毁过程
class TestObj
{
public:
    // 构造函数：创建对象时输出提示
    TestObj()
    {
        cout << "[构造] 创建了一个TestObj对象\n";
    }

    // 析构函数：销毁对象时输出提示（核心观察点）
    ~TestObj()
    {
        cout << "[析构] 销毁了一个TestObj对象\n";
    }
};

int main()
{
    cout << "============= 1. 单个对象：new → delete（正确用法） =============\n";
    // 用new创建**单个**TestObj对象
    TestObj *single_obj = new TestObj;
    // 用delete释放单个对象：会调用1次析构函数
    delete single_obj;
    cout << "---------------------------------------------------------\n\n";

    cout << "============= 2. 数组对象：new[] → delete[]（正确用法） =============\n";
    // 用new[]创建**数组**（3个TestObj对象）
    TestObj *obj_array = new TestObj[3];
    // 用delete[]释放数组：会调用3次析构函数（每个元素都被正确销毁）
    delete[] obj_array;
    cout << "---------------------------------------------------------\n\n";

    cout << "============= 3. 数组对象：new[] → delete（错误用法，未定义行为） =============\n";
    // 用new[]创建数组（3个TestObj对象）
    TestObj *wrong_array = new TestObj[3];
    // 错误：用delete释放数组（而非delete[]）
    // 后果：仅调用第1个元素的析构函数，剩下2个元素的析构被跳过（内存泄漏）
    // 注意：这属于“未定义行为”，不同编译器可能有不同表现（如崩溃、内存泄漏）
    delete wrong_array;
    cout << "---------------------------------------------------------\n\n";

    return 0;
}