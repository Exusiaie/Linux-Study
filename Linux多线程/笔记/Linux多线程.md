# 一: Linux多线程常见问题

### Q:为什么要多线程

要充分利用进程资源做到<font color = red>**并行**</font>

>并发和并行:
>
>并发: 在同一个大的时间段内, 有多个任务正在执行
>
>并行: 在同一时刻内, 有多个任务正在执行
>
>单核CPU只能做到并发, 多核CPU可以做到100%并行



### Q: 进程和线程的区别

进程:

-    ==操作系统分配资源的基本单位==
-   拥有独立的内存空间和系统资源(os分配的资源)
-   进程与进程相互独立, 一个进程崩溃不会导致所有的进程崩溃
-   进程创建销毁切换的开销比较大

线程: 

-   ==CPU调度的基本单位==
-   线程共享进程的内存空间, 子线程拥有自己独立的==栈空间==
-   共享进程资源, 一个线程崩溃可能导致整个进程崩溃
-   进程创建销毁切换的开销比较小

<img src="D:\MarkDown\08_Linux多线程\assets\image-20250722203058926.png" alt="image-20250722203058926" style="zoom:50%;" />



### Q: 一个进程可以创建的子线程有多少个

32位系统 **->** 寻址空间2^32=4G **->** 

虚拟地址空间的六区( 内核区, 栈区, 堆区, 全局静态区, 文字常量区, 程序代码区 ) **->**

内核区占用1G, 可用空间3G **->** 

可创建的子线程: 在32位系统中: ==3G / 一个函数的栈空间的大小==

 

### Q: 一般情况下, 一个进程要开辟多少个线程是比较合理的

与CPU核心数量有关, 一般情况下, ==1核对应着1-2个线程== ( 8核16线程 )



### Q: 线程间的关系

使用`pthread_create`函数创建子线程，这些子线程和主线程（`main`函数的线程）会**并行执行**。

main线程是进程的入口，是主线程，当main线程退出时，子线程也结束了，其他的线程都称为子线程

子线程内部依然可以创建子线程

所有的子线程在进程内部都是==平等==的

所有线程==共享同一进程的资源==，子线程之间可以互相等待，但是子线程不能等待主线程，子线程可以用`join`函数让主线程等待子线程结束后再结束

<img src="D:\MarkDown\08_Linux多线程\assets\image-20250722201301561.png" alt="image-20250722201301561" style="zoom:50%;" />



### Q: 在进程内部如何区分不同的子线程?

我们使用线程ID(Thread id, 简称TID)来区分不同的子线程

`pthread_t`类型: 

​	使用 `pthread_create` 函数创建一个新线程时，它会返回一个 `pthread_t` 类型的值。这个 `pthread_t` 就是==线程ID==，你可以把它看作是线程在用户空间的一个“句柄”或“名字”。在大多数Linux系统上，`pthread_t` 通常是 `unsigned long` 类型    // 但在不同的系统或库实现中，它可能是一个结构体或指向某个内部结构的指针

`pthread_self`函数:

​	线程可以调用这个函数来知道自己的ID是什么, 也就是获取自己的`pthread_t`



### Q: 对于Linux操作系统内核来说, 线程是什么

​	在Linux中，从内核的角度来看，没有线程的概念，只有进程, 线程 (子线程和主线程都是) 是以(LWP)轻量级进程的形式存在的，都是有相同的结构体`struct task_struct`俗称的进程控制块PCB来表示的





# 二: 线程创建函数 int pthread_create( )

```c++
#include <pthread.h>

int pthread_create(pthread_t *restrict thread,
                   const pthread_attr_t *restrict attr,
                   void *(*start_routine)(void *),
                   void *restrict arg);
```

`pthread_t *restrict thread` 

​	当函数成功返回时，它会被新创建的 <font color = red>**子线程的 ID**</font> 填充。这个 ID 可以用来引用或操作这个线程（例如，用 pthread_join()等待它结束，或者用 pthread_cancel()取消它）。pthread_t是一个无符号长整型

`const pthread_attr_t *restrict attr`

​	用于<font color = red>**指定新创建线程的属性**</font>, 简单传入NULL就以默认属性创建

`void *(*start_routine)(void *)`

​	是一个函数指针, <font color = red>**指向一个新创建线程将要执行的函数 ( 线程入口函数 )**</font>. 这个被指向的函数必须接收一个 void* 类型的参数, 并返回一个 void* 类型的值. 当被创建的新线程启动时, 这个线程会立即执行 start_routine 所指向的函数

`void *restrict arg`

​	这是<font color = red>**传递给线程入口函数的参数**</font>. 需要向线程入口函数传递多个参数的方法: 创建一个==结构体==来包含所有参数，然后将结构体的指针作为 arg 传递. 如果不需要传递参数可以直接传入NULL.

`返回值`

​	成功返回0, 失败返回错误码







# 三: 主线程等待函数 pthread_join( )

pthread_join是一个阻塞式函数，当子线程没有退出时，会一直阻塞

```c++
#include <pthread.h>
int pthread_join(pthread_t thread, void **retval);
```

`pthread_t thread`

​	子线程的id

`void **retval`

​	获取子线程函数退出时的返回值, 不需要时直接设置为NULL





# 四: 综合实例

```c++
#include <iostream>
#include <memory>
#include <pthread.h>

using namespace std;

void* pthread_function(void *arg){
    printf("线程入口函数已经执行\n");
    int *number = static_cast<int*>(arg); // arg是主线程传递过来的一个void*类型指针, 需要进行类型转化
    printf("打印从主线程获取到的参数: %d \n", *number);
    cout << "使用函数获取自己的tid: " << pthread_self() << endl;

    return NULL;
}

int main(){
    pthread_t tid;
    // int number = 7777; // 直接传递栈地址的操作不安全
    // // 安全的做法是创建一个堆对象, 并使用智能指针托管
    unique_ptr<int> up = make_unique<int>(7770721);

    int ret = pthread_create(&tid, NULL, pthread_function, up.get());
    printf("子线程已创建, 返回值: %d \n", ret);
    cout << "主线程中得到的tid: " << tid << endl;
    pthread_join(tid, NULL);
    
    printf("主线程已经等待子线程执行完毕, 即将退出\n");
    return 0;
}
```

<img src="D:\MarkDown\08_Linux多线程\assets\image-20250722220642785.png" alt="image-20250722220642785" style="zoom: 67%;" />





# 五: 封装线程类

###  函数指针

```c++
返回类型 (*指针变量名)(参数类型1, 参数类型2, ...);
int (*pAdd)(int, int); // 函数指针
int *pAdd(int, int);   // 返回值是指针的函数
```

从上面可以看见函数指针的语法主要是指针变量外要加括号, 这样编译器可以区分是函数指针还是返回值是指针的函数





###  Thread类

思路: 线程的作用是用来执行一个任务, 但是具体的任务是什么不知道, 如果用面向对象的方式来表示, 就是需要设计一个纯虚函数

>基类的析构函数设置为虚函数
>
>如果不设置为虚函数, 派生类就无法使用析构函数

// **线程是一种系统资源, 不能进行拷贝或者赋值**, 要表达对象语义

// 所谓的复制控制就是要调用拷贝构造函数和赋值运算符重载函数

// 让类继承自定义的NonCopyable类来实现对象语义

// 定义了protected构造函数的类被称为抽象类

![image-20250722205906403](D:\MarkDown\08_Linux多线程\assets\image-20250722205906403.png)





# 六: 同步和互斥



### 初始化互斥锁

**静态初始化:**

```c++
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
```

这种方法适用于在编译时已知互斥锁的情况，且通常用于全局或静态变量。

**动态初始化：**

使用初始化函数

```c++
pthread_mutex_t my_mutex;
int ret = pthread_mutex_init(&my_mutex, NULL);
// 检查 ret 是否为 0 以确保初始化成功
```

`pthread_mutex_init()` 函数允许你指定互斥锁的属性 (通过第二个参数 `attr`)。如果为 `NULL`，则使用默认属性。



### 加锁

