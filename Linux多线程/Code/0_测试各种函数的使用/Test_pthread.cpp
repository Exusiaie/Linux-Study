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


