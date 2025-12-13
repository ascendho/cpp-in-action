// 自定义字符串类头文件，实现深拷贝语义的字符串管理
// 核心特性：封装动态字符数组资源、支持C风格字符串互转、重载相等比较运算符
// 设计思路：遵循RAII（资源获取即初始化）原则，通过深拷贝避免浅拷贝导致的内存问题
#ifndef STRING_HPP
#define STRING_HPP

// 包含标准C字符串操作头文件，提供memcmp(内存比较)/memcpy(内存拷贝)/strlen(字符串长度)/size_t(无符号长度类型)
#include <string.h>  

// 自定义String类，封装字符串的存储、拷贝、比较等核心操作
class String {
public:
    // 默认构造函数：初始化空字符串
    // 初始状态：指针置空（无动态内存分配），长度为0
    String() : ptr_(nullptr), len_(0) {}

    // 带C风格字符串的构造函数：从const char*类型初始化字符串
    // 参数s：C风格字符串（以'\0'结尾）
    // 核心逻辑：深拷贝s的内容，len_记录有效字符长度（不含'\0'），分配内存时+1预留'\0'的空间
    String(const char* s) : ptr_(nullptr), len_(strlen(s))
    {
        // 仅当字符串非空时分配内存（避免空指针操作）
        if (len_ != 0) {
            ptr_ = new char[len_ + 1];  // +1 为字符串终止符'\0'预留空间
            memcpy(ptr_, s, len_ + 1);  // 拷贝包括'\0'在内的所有字符，保证C风格字符串完整性
        }
    }

    // 拷贝构造函数：实现深拷贝，避免浅拷贝导致的多个对象共享同一块内存
    // 参数rhs：待拷贝的String对象（const保证不修改源对象）
    // 核心逻辑：独立分配内存，拷贝源对象的字符串内容，与源对象解耦
    String(const String& rhs) : ptr_(nullptr), len_(rhs.len_)
    {
        // 仅当源对象非空时分配内存
        if (len_ != 0) {
            ptr_ = new char[rhs.len_ + 1];  // 预留'\0'空间
            memcpy(ptr_, rhs.ptr_, len_ + 1);  // 深拷贝源对象的字符串（含'\0'）
        }
    }

    // 赋值运算符重载：实现拷贝赋值，遵循"自赋值检查+先分配后释放"的异常安全原则
    // 参数rhs：待赋值的String对象（const保证不修改源对象）
    // 返回值：当前对象的引用（支持链式赋值，如a = b = c）
    // 核心逻辑：避免自赋值导致的资源提前释放，先分配新内存再释放旧内存（防止分配失败时丢失原数据）
    String& operator=(const String& rhs)
    {
        // 自赋值检查：若当前对象与源对象是同一个，直接返回（避免释放自身内存后拷贝）
        if (this != &rhs) {
            char* ptr = nullptr;
            // 源对象非空时，分配新内存并拷贝内容
            if (rhs.len_ != 0) {
                ptr = new char[rhs.len_ + 1];
                memcpy(ptr, rhs.ptr_, len_ + 1);  // 拷贝含'\0'的完整字符串
            }
            delete[] ptr_;  // 释放当前对象的旧内存，避免内存泄漏
            ptr_ = ptr;     // 指向新分配的内存
            len_ = rhs.len_;// 更新字符串长度
        }
        return *this;  // 返回自身引用，支持链式赋值
    }

    // 析构函数：释放动态分配的字符数组资源，遵循RAII原则
    // 核心作用：对象销毁时自动释放内存，避免内存泄漏
    ~String()
    {
        delete[] ptr_;  // 释放数组需用delete[]，与new char[]匹配
    }

    // 获取C风格字符串指针（const版本，保证不修改内部资源）
    // 返回值：指向内部字符数组的const指针（以'\0'结尾），可直接用于C标准字符串函数
    const char* c_str() const
    {
        return ptr_;
    }

    // 获取字符串有效长度（不含终止符'\0'）
    // 返回值：无符号整数size_t，代表字符串的字符个数
    size_t size() const
    {
        return len_;
    }

    // 重新赋值为新的C风格字符串
    // 参数s：新的C风格字符串（以'\0'结尾）
    void assign(const char* s);

    // 静态成员函数：比较两个String对象是否相等（高效实现）
    // 参数lhs/rhs：待比较的两个String对象（const保证不修改）
    // 返回值：相等返回true，否则返回false
    // 优化逻辑：先比较长度（长度不同直接不相等），再用memcmp逐字节比较（比strcmp更高效，无需遍历到'\0'）
    static bool equals(const String& lhs, const String& rhs)
    {
        // 长度不同，直接返回false（快速失败）
        if (lhs.len_ != rhs.len_) {
            return false;
        }
        // 长度相同时，逐字节比较有效字符（len_个字节，无需比较'\0'）
        return memcmp(lhs.ptr_, rhs.ptr_, lhs.len_) == 0;
    }

private:
    char*  ptr_;   // 指向动态分配的字符数组，存储以'\0'结尾的字符串，空字符串时为nullptr
    size_t len_;   // 字符串有效长度（不含终止符'\0'），空字符串时为0
};

// 内联成员函数：assign的实现（内联提升调用效率）
// 核心逻辑：与赋值运算符类似，先分配新内存、拷贝内容，再释放旧内存，最后更新指针和长度
inline void String::assign(const char* s)
{
    size_t len = strlen(s);  // 获取新字符串的有效长度（不含'\0'）
    char* ptr = nullptr;
    // 新字符串非空时分配内存（含'\0'）
    if (len != 0) {
        ptr = new char[len + 1];
        memcpy(ptr, s, len + 1);  // 拷贝含'\0'的完整字符串
    }
    delete[] ptr_;  // 释放旧内存
    ptr_ = ptr;     // 指向新内存
    len_ = len;     // 更新长度
}

// 全局相等比较运算符重载：调用String::equals实现，支持自然的相等判断（如a == b）
// 参数lhs/rhs：待比较的两个String对象（const保证不修改）
// 返回值：相等返回true，否则返回false
inline bool operator==(const String& lhs, const String& rhs)
{
    return String::equals(lhs, rhs);
}

#endif // STRING_HPP  // 结束头文件保护宏，防止重复包含