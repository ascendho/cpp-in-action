// To compile: g++ -o memcmp memcmp.cpp
// To run:     ./memcmp
#include <iostream>
#include <cstring>
#include <string>  // 仅用于场景3的部分字节字符串展示
using namespace std;

/* 自定义结构体：演示结构体内存比较 */
struct Product {
    char name[10];  // 商品名称
    float price;    // 价格
    int stock;      // 库存
};

// int memcmp(const void* ptr1, const void* ptr2, size_t n);
// memcmp 是 C/C++ 标准库函数（定义在 <cstring> 头文件），核心作用是按字节逐位比较两块内存区域的内容，不关心数据类型，仅以无符号字符（unsigned char）为单位对比，可用于判断任意类型内存块的相等性或字节序大小关系。

int main() {
    // 场景1：比较完全相等的int类型内存块
    int num1 = 0x12345678;
    int num2 = 0x12345678;
    int cmp_int = memcmp(&num1, &num2, sizeof(int));
    cout << "【场景1】比较相等的int内存块" << endl;
    cout << "memcmp返回值：" << cmp_int << " → " 
         << (cmp_int == 0 ? "内存完全相等" : "内存不相等") << endl << endl;

    // 场景2：比较不相等的字符数组（逐字节对比）
    char str1[] = "Apple";
    char str2[] = "Applz";
    int cmp_str = memcmp(str1, str2, sizeof(str1));
    cout << "【场景2】比较不相等的字符数组" << endl;
    cout << "str1: " << str1 << ", str2: " << str2 << endl;
    cout << "memcmp返回值：" << cmp_str << " → " 
         << (cmp_str < 0 ? "str1内存字节序更小" : "str1内存字节序更大") << endl << endl;

    // 场景3：比较部分字节（前N字节相等，后续不同）
    char buf1[] = "HelloWorld";
    char buf2[] = "HelloC++";
    // 仅比较前5字节（"Hello"部分）
    int cmp_part1 = memcmp(buf1, buf2, 5);
    cout << "【场景3】比较部分字节（前5字节）" << endl;
    cout << "buf1前5字节：" << string(buf1, 5) << ", buf2前5字节：" << string(buf2, 5) << endl;
    cout << "memcmp返回值：" << cmp_part1 << " → " 
         << (cmp_part1 == 0 ? "前5字节完全相等" : "前5字节不相等") << endl;
    // 比较前6字节（第6字节：'W' vs 'C'）
    int cmp_part2 = memcmp(buf1, buf2, 6);
    cout << "比较前6字节：" << endl;
    cout << "memcmp返回值：" << cmp_part2 << " → " 
         << (cmp_part2 > 0 ? "buf1第6字节更大" : "buf1第6字节更小") << endl << endl;

    // 场景4：比较结构体内存（整体字节对比）
    Product prod1 = {"Phone", 2999.99f, 100};
    Product prod2 = {"Phone", 2999.99f, 100};
    Product prod3 = {"Phone", 2999.99f, 99};
    int cmp_prod1 = memcmp(&prod1, &prod2, sizeof(Product));
    int cmp_prod2 = memcmp(&prod1, &prod3, sizeof(Product));
    cout << "【场景4】比较结构体内存" << endl;
    cout << "prod1 vs prod2 返回值：" << cmp_prod1 << " → " 
         << (cmp_prod1 == 0 ? "结构体内存相等" : "结构体内存不相等") << endl;
    cout << "prod1 vs prod3 返回值：" << cmp_prod2 << " → " 
         << (cmp_prod2 != 0 ? "结构体内存不相等" : "结构体内存相等") << endl << endl;

    // 场景5：有符号数的无符号比较特性（memcmp的关键注意点）
    int neg_num1 = -1;  // 32位系统下二进制：0xFFFFFFFF（无符号值4294967295）
    int neg_num2 = 1;   // 32位系统下二进制：0x00000001（无符号值1）
    int cmp_neg = memcmp(&neg_num1, &neg_num2, sizeof(int));
    cout << "【场景5】有符号数的无符号比较特性" << endl;
    cout << "neg_num1(-1)内存：0xFFFFFFFF（无符号值4294967295）" << endl;
    cout << "neg_num2(1)内存：0x00000001（无符号值1）" << endl;
    cout << "memcmp返回值：" << cmp_neg << " → " 
         << (cmp_neg > 0 ? "neg_num1内存字节序更大" : "neg_num1内存字节序更小") << endl << endl;

    

    // 场景6：memcmp 与 strcmp 的核心区别（修正转义字符问题，手动构造含\0的数组）
    /* 
       手动初始化数组，确保：
       - str3: Test + \0 + 1 + 2 + 3 + \0（总长度8）
       - str4: Test + \0 + 4 + 5 + 6 + \0（总长度8）
       避免字符串字面量中\0后接数字被解析为八进制转义的问题
    */
    char str3[8] = {'T','e','s','t','\0','1','2','3'};
    char str4[8] = {'T','e','s','t','\0','4','5','6'};
    
    int cmp_strcmp = strcmp(str3, str4);  // strcmp遇到\0立即停止比较
    int cmp_memcmp = memcmp(str3, str4, sizeof(str3));  // memcmp对比全部8字节
    
    cout << "【场景6】memcmp vs strcmp 区别" << endl;
    cout << "str3（可见部分）: " << str3 << endl;  // 输出到\0为止，即Test
    cout << "str4（可见部分）: " << str4 << endl;  // 输出到\0为止，即Test
    cout << "strcmp返回值：" << cmp_strcmp << " → " 
         << (cmp_strcmp == 0 ? "strcmp认为相等（仅比较到\\0）" : "strcmp认为不相等") << endl;
    cout << "memcmp返回值：" << cmp_memcmp << " → " 
         << (cmp_memcmp != 0 ? "memcmp认为不相等（对比全部字节）" : "memcmp认为相等") << endl;

    return 0;
}