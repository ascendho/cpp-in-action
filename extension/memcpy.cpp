// To compile: g++ -o memcpy memcpy.cpp
// To run:     ./memcpy
#include <iostream>
#include <cstring> // 包含memcpy/memmove定义
#include <cstdio>  // 用于十六进制打印

using namespace std;

// 自定义结构体：演示结构体拷贝
struct Student
{
    char name[20]; // 姓名
    int age;       // 年龄
    float score;   // 分数
};

// void* memcpy(void* dest, const void* src, size_t n);
// memcpy 是 C/C++ 标准库函数（定义在 <cstring> 头文件），核心作用是按字节数拷贝内存块—— 它不关心数据类型，仅对二进制字节流进行操作，可拷贝任意类型数据（基本类型、数组、结构体等），区别于仅拷贝以 \0 结尾字符串的 strcpy。

int main()
{
    // 示例1：拷贝单个基本类型（int）
    int src_num = 100;
    int dest_num = 0;
    memcpy(&dest_num, &src_num, sizeof(int)); // 拷贝4个字节（int占4字节）
    cout << "【示例1】拷贝单个int" << endl;
    cout << "源值：" << src_num << " | 目标值：" << dest_num << endl
         << endl;

    // 示例2：拷贝含'\0'的字符数组（对比strcpy）
    char src_str[] = "Hello\0World"; // 中间含'\0'，strcpy会在此停止
    char dest_str[20] = {0};
    // memcpy不受'\0'影响，拷贝全部指定字节（sizeof包含末尾'\0'）
    memcpy(dest_str, src_str, sizeof(src_str));
    cout << "【示例2】拷贝含\\0的字符数组" << endl;
    cout << "源数组字节数：" << sizeof(src_str) << endl;
    cout << "目标数组内容（十六进制）：";
    for (int i = 0; i < sizeof(src_str); ++i)
    {
        printf("%02X ", (unsigned char)dest_str[i]); // 按字节打印
    }
    cout << endl
         << endl;

    // 示例3：拷贝int数组
    int src_arr[] = {1, 2, 3, 4, 5};
    int dest_arr[5] = {0};
    // 拷贝整个数组：元素数*单个元素字节数 = sizeof(src_arr)
    memcpy(dest_arr, src_arr, sizeof(src_arr));
    cout << "【示例3】拷贝int数组" << endl;
    cout << "目标数组元素：";
    for (int num : dest_arr)
    {
        cout << num << " ";
    }
    cout << endl
         << endl;

    // 示例4：拷贝结构体
    Student src_stu = {"ZhangSan", 18, 95.5};
    Student dest_stu = {0};
    memcpy(&dest_stu, &src_stu, sizeof(Student)); // 拷贝整个结构体字节
    cout << "【示例4】拷贝结构体" << endl;
    cout << "姓名：" << dest_stu.name
         << " | 年龄：" << dest_stu.age
         << " | 分数：" << dest_stu.score << endl
         << endl;

    // 示例5：内存重叠（memcpy未定义行为，对比memmove）
    int overlap_arr[] = {1, 2, 3, 4, 5};
    cout << "【示例5】内存重叠（memcpy vs memmove）" << endl;
    cout << "原数组：";
    for (int num : overlap_arr)
        cout << num << " ";
    cout << endl;

    // memcpy拷贝重叠内存：结果未定义（不同编译器可能不同）
    memcpy(overlap_arr, overlap_arr + 1, 3 * sizeof(int)); // 拷贝3个int（12字节）
    cout << "memcpy拷贝后：";
    for (int num : overlap_arr)
        cout << num << " ";
    cout << endl;

    // memmove安全处理重叠内存
    int move_arr[] = {1, 2, 3, 4, 5};
    memmove(move_arr, move_arr + 1, 3 * sizeof(int));
    cout << "memmove拷贝后：";
    for (int num : move_arr)
        cout << num << " ";
    cout << endl;

    return 0;
}