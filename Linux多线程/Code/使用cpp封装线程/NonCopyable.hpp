#ifndef __NonCopyable_H__
#define __NonCopyable_H__

namespace Ning
{
//名字本身表达的含义就是不可以复制
class NonCopyable
{
//定义了protected构造函数的类称为抽象类
protected:
    NonCopyable() {}
    ~NonCopyable(){}

    NonCopyable(const NonCopyable &) = delete;
    NonCopyable & operator=(const NonCopyable &) = delete;
};

}//end of namespace wd

#endif


