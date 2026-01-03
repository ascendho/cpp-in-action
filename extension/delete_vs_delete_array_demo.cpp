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
    /*
     * 内存布局（假设对象大小为 16 字节）：
     *
     * [ 16字节：TestObj 对象数据 ]
     * ↑
     * single_obj 指向这里（malloc返回的地址）
     *
     * delete single_obj:
     * 1. 调用一次析构函数
     * 2. 释放 single_obj 指向的地址（正确，因为这是 malloc 返回的地址）
     */
    TestObj *single_obj = new TestObj;
    delete single_obj; // ✅ 正确匹配
    cout << "---------------------------------------------------------\n\n";

    cout << "============= 2. 数组对象：new[] → delete[]（正确用法） =============\n";
    /*
     * 内存布局（假设每个对象 16 字节，额外信息 4 字节）：
     *
     * [ 4字节：元素个数 (3) ][ 16字节：对象1 ][ 16字节：对象2 ][ 16字节：对象3 ]
     *                        ↑
     *                        obj_array 指向这里（第一个对象的起始地址）
     *
     * delete[] obj_array:
     * 1. 向前偏移 4 字节，读取元素个数 (3)
     * 2. 从 obj_array 开始，调用 3 次析构函数
     * 3. 释放最前面的地址（malloc返回的地址）
     */
    TestObj *obj_array = new TestObj[3];
    delete[] obj_array; // ✅ 正确匹配
    cout << "---------------------------------------------------------\n\n";

    cout << "============= 3. 数组对象：new[] → delete（错误用法，未定义行为） =============\n";
    /*
     * 内存布局同上：
     * [ 4字节：元素个数 (3) ][ 对象1 ][ 对象2 ][ 对象3 ]
     *                        ↑
     *                        wrong_array 指向这里
     *
     * delete wrong_array:
     * 1. 以为 wrong_array 是单个对象的指针，只调用一次析构函数（只销毁第一个对象）
     * 2. 直接释放 wrong_array 指向的地址（这个地址不是 malloc 返回的地址，而是跳过了前面 4 字节）
     * 3. malloc 检测到释放的指针不是它分配的起始地址 → 报错：pointer being freed was not allocated
     */
    TestObj *wrong_array = new TestObj[3];
    delete wrong_array; // ❌ 错误匹配：会崩溃
    cout << "---------------------------------------------------------\n\n";

    return 0;
}