// 目的是为了禁止拷贝和赋值, 实现对象语义

#ifndef __NonCopyable_H__
#define __NonCopyable_H__

namespace Ning{
class NonCopyable{
// 将构造函数设为保护或私有, 可以阻止直接实力化这个类, 可以实现抽象类
protected:
    NonCopyable() {}
    ~NonCopyable() {}
    // 删除拷贝构造
    NonCopyable(const NonCopyable &) = delete;
    // 删除赋值运算符
    NonCopyable & operator= (const NonCopyable &) = delete;
};

} 



#endif

