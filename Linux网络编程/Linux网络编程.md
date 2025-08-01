# 一: 计网分层体系

物理层 / 数据链路层 / 网络层(路由器, 通信子网) / 传输层(TCP,UDP, 资源子网) / 会话层 / 表示层 / 应用层

传输层: 端到端的传输	网络层: 点对点的通信

每一层的数据包的名字都不一样

每一层的协议也都不一样: 应用层: HTTP / 传输层: TCP,UDP / 网络层: IP / 数据链路层: ARP

![image-20250728202926728](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250728202926728.png)





# 二: TCP协议 (传输层)

面向连接, 可靠传输, 全双工, 字节流, 流量控制, 拥塞控制

## 三次握手

1 第一次握手:  客户端发送连接请求

​		**客户端** 向 **服务器** 发送一个 <font color = red>**SYN**</font> 报文段。

​		这个报文段的目的是请求建立连接，并携带发送端选择的**初始序列号** ，例如 `seq = x`。SYN 标志位被设置为 1。

2 第二次握手:  服务器确认+同意连接

​	**服务器** 收到客户端的 SYN 报文段后，会回复一个 <font color = red>**SYN+ACK**</font> 报文段。这个报文段表示服务器已收到客户端的连接请求并同意建立连接。

报文段中包含服务器**自己的初始序列号(SYN)**，例如 `seq = y`。还有, 服务器会发送一个 **确认号 (ACK)**，其值为客户端的初始序列号 `x + 1` ，即 `ACK = x + 1`。这表示服务器已经收到了 `x` 及其之前的所有数据，并**期待**客户端下一个报文段的序列号从 `x + 1` 开始。`SYN` 和 ACK 标志位都被设置为 1。

3 第三次握手: 客户端确认连接

​        **客户端**收到服务器的 SYN + ACK 报文段后, 会回复一个 <font color = red>**ACK**</font> 报文段, 其值为服务器的初始序列号 `y + 1` , 即 **`ACK = y + 1`**。这个报文段表示客户端已经收到了服务器的第二次握手，从而确认了服务器的初始序列号 `y`, 并期待服务器下一个报文段的序列号从 `y + 1` 开始。

​	同时，客户端会发送自己的序列号，即**第二次握手服务器所期待的序列号 `x + 1`**，即 **`seq = x + 1`**。ACK 标志位被设置为 1。

## 四次挥手

假设 **客户端** 先发起关闭请求（主动关闭方），**服务器** 被动关闭。

1 第一次挥手：客户端发起关闭请求 (FIN)

​	**主动关闭方（客户端）** 的应用进程调用 `close()` 函数，向 TCP 发送一个 **FIN 报文段**。这个 `FIN` 报文段的目的是通知对方：“我要关闭发送了”

​	报文段中包含一个序列号 **`SEQ = u`** (假设 `u` 是客户端已发送的最后一个字节的序列号加 1)。`FIN` 标志位被设置为 `1`。此时，客户端进入 **FIN-WAIT-1** 状态，等待服务器的确认。



2 第二次挥手：服务器确认关闭请求 (ACK)

​	**被动关闭方（服务器）** 收到客户端的 `FIN` 报文段后，会立即回复一个 **ACK 报文段**。作用: “服务器确认” ,  报文段中包含服务器自己的序列号 **`seq = v`**。确认号 **`ACK = u + 1`**，表示已确认收到客户端的 `FIN` 报文段，并期望收到客户端下一个序列号为 `u + 1` 的数据（尽管客户端可能不会再发送了，这只是一个确认机制）。`ACK` 标志位被设置为 `1`。

​	此时，服务器进入 **CLOSE-WAIT** 状态。在这个状态下，服务器知道客户端已经没有数据要发给自己了，但服务器可能还有数据没有发送给客户端。因此，服务器会等待其应用进程处理完所有待发送的数据。



3 第三次挥手：服务器发送关闭请求 (FIN)

​	当 **被动关闭方（服务器）** 确认所有数据都已发送给客户端后，其应用进程也会调用 `close()` 函数，向 TCP 发送一个 **FIN 报文段**。这个 `FIN` 报文段的目的是通知客户端：“我也要关闭发送了”

​	报文段中包含服务器自己的序列号 **`seq = w`** (假设 `w` 是服务器已发送的最后一个字节的序列号加 1)。确认号 **`ACK = u + 1`**（与第二次挥手相同，因为它依然是确认客户端的 `FIN`）。`FIN` 标志位和 `ACK` 标志位都被设置为 `1`。此时，服务器进入 **LAST-ACK** 状态，等待客户端的最终确认。



4 第四次挥手：客户端确认关闭请求 (ACK)

​	**主动关闭方（客户端）** 收到服务器的 `FIN` 报文段后，会回复一个 **ACK 报文段**。作用: “客户端确认”

​	报文段中包含客户端的序列号 **`SEQ = u + 1`**。确认号 **`ACK = w + 1`**，表示已确认收到服务器的 `FIN` 报文段，并期望收到服务器下一个序列号为 `w + 1` 的数据。`ACK` 标志位被设置为 `1`。

​	此时，客户端进入 **TIME-WAIT** 状态。这个状态会持续一段固定的时间（通常是 2MSL，即最长报文段寿命的两倍），然后客户端才进入 **CLOSED** 状态。当服务器收到这个最终的 `ACK` 报文段后，也会立即进入 **CLOSED** 状态。



<img src="C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250728203636810.png" alt="image-20250728203636810" style="zoom: 50%;" />

#### 为什么有TIME_WAIT

TIME_WAIT 是：TCP 正常断开后的状态, 出现在主动关闭连接的一方, 它会等待 2×MSL 时间，确保连接彻底关闭
作用是

1.   确保对方收到最后的ACK, 如果最后那个ACK丢失, 挥手的那一方还要重发FIN;
2.   防止旧的链接数据影响新连接, 等待旧数据清理完毕 . 这种状态会占用端口









# 三: TCP建立连接, 套接字编程

## 套接字

**通信的“端点”：** 两个设备要通信，就像两个人要通电话，都需要一个“电话”作为端点。套接字就是这个“电话”，它是数据在网络中传输的起点和终点。

当你创建一个套接字时，你实际上是向操作系统请求一个用于网络通信的资源，这个资源会提供一系列函数（如 `socket()`, `bind()`, `listen()`, `connect()`, `send()`, `recv()`, `close()` 等），它们共同构成了进行网络通信的工具集。

## 文件描述符

套接字在Linux中以文件描述符的形式存在, 使用`socket()`创建一个套接字时, os会返回一个int值, 就是套接字对应的文件描述符

>   在Linux系统中一切皆文件, 文件描述符是访问文件的“索引”, 每个打开的文件在进程中都有一个对应的文件描述符, 012对应标准输入标准输出和标准错误, 所以文件的文件描述符是从3开始的

流程:

<img src="C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729091921372.png" alt="image-20250729091921372" style="zoom:50%;" />

<img src="C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729092114449.png" alt="image-20250729092114449" style="zoom:67%;" />

<img src="C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729102429175.png" alt="image-20250729102429175" style="zoom:50%;" />

## socket( )

![image-20250729092215400](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729092215400.png)

```c++
int client_fd = socket(AF_INET, SOCK_STREAM, 0); // 使用ipv4和TCP协议
if(client_fd < 0){
	perror("socket ERROR");
}   
```

socket函数返回的是一个整型数据，代表的就是一个套接字, 这是一个文件描述符, 代表了一个操作系统内核数据结构

每个Linux进程在启动时都会默认开启三个文件描述符: 标准输入0, 标准输出1, 标准错误2



## bind( )

![image-20250729092518134](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729092518134.png)

>   **sockfd:**
>
>   这是一个**套接字文件描述符**，由 `socket()` 函数返回
>
>   
>
>   **addr:**
>
>   指向 struct sockaddr 结构体的指针。这个结构体包含了要绑定的本地 IP 地址和端口号信息
>
>   一般只有**两个选项**: struct sockaddr_in（用于 IPv4）或 struct sockaddr_in6（用于 IPv6）
>
>   
>
>   **socklen_t addrlen:**
>
>   这是 addr 参数所指向结构体的**大小**。一般使用 sizeof() 运算符来获取这个大小，例如 sizeof(struct sockaddr_in)
>
>   
>
>   **返回值:**
>
>   成功返回0, 失败返回-1并设置ERROR







当服务器开始运行时需要将sfd和一个网络地址绑定在一块, 该网络地址需要暴露给客户端

==和std::bind是两个函数==



## listen( )

![image-20250729093247978](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729093247978.png)

三次握手与listen函数有很大关系

backlog的值设置了服务器能同时处理的连接数, 连接数是backlog + 1

**半连接队列和全连接队列:**

​	在TCP建立连接的过程中, 服务器端的监听套接字(listen)会维护两个非常重要的队列, 半连接队列和全连接队列.

​	**半连接队列** 存储的是在 TCP 三次握手过程中，服务器已经收到了客户端的 SYN 报文，并回复了 SYN+ACK 报文，但尚未收到客户端最终 ACK 报文的连接请求。这些连接处于 SYN_RCVD (SYN_RECEIVED) 状态。

​	**全连接队列** 存储的是已经完成了 TCP 三次握手过程，处于 ESTABLISHED 状态的连接。这些连接已经准备好被服务器的应用进程调用 `accept()` 函数取出并进行数据通信。当半连接队列中的报文完成了第三次握手, 就会从半连接队列移入全连接队列





>这两个队列协同工作，共同管理 TCP 连接的建立过程：
>
>1.  客户端发起连接（SYN）。
>2.  服务器接收 SYN，发送 SYN+ACK，并将连接放入 **半连接队列** (SYN_RCVD)。
>3.  客户端回复 ACK。
>4.  服务器收到 ACK，将连接从半连接队列移到 **全连接队列** (ESTABLISHED)。
>5.  服务器的应用进程调用 `accept()`，从 **全连接队列** 中取出连接，开始数据通信。

<img src="C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729095623481.png" alt="image-20250729095623481" style="zoom:67%;" />



## accept( )

![image-20250729095929381](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729095929381.png)

`accept`会从全连接队列中取下一个连接，返回的是一个新的套接字<font color = red>**newFd**</font>，之后的通信不再使用sfd (sfd表示的是监听套接字, 不是用来与客户端进行通信的)

`accept`是一个阻塞式的函数，没有新连接到来时，就傻傻地等待



## connect( )

![image-20250729100957494](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729100957494.png)



## recv( )

![image-20250729101219692](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729101219692.png)

`recv`函数的作用是从内核接收缓冲区中获取数据，数据会被放到第二个参数所指的空间

默认情况下recv函数是一个阻塞式函数

>int sockfd参数:  netfd/peerfd  代表已经建立好的连接
>
>void *buf, size_t len参数: 指定了一个数组的空间，用户态指定的接收缓冲区
>
>int flags参数: MSG_DONTWAIT  非阻塞
>
>​	   MSG_PEEK  只拷贝，但并不移走内核接收缓冲区中的数据
>
>
>
>返回值: 
>
>1.大于0  表示实际接收的数据	
>
>2.等于0  **表示连接已经断开**	
>
>3.小于0  表示发生了错误







## send( )

![image-20250729101903998](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729101903998.png)

send函数只是将用户态的发送缓冲区中的数据拷贝到内核态发送缓冲区，

真正的发送操作是内核协议栈来完成的

>const void *buf, size_t len参数: 用户态指定的发送缓冲区和它的长度
>
>int flags参数: 一般设为0
>
>
>
>返回值:
>
>1.   大于等于0， 表示拷贝到内核发送缓冲区的数据的字节数
>
>2.   小于0， 表示发生了错误





# 四: IO多路复用模型, epoll

![image-20250729201723033](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729201723033.png)





**IO**多路复用是一种同步IO模型，实现一个线程可以监听多个**文件描述符**；

一旦某个文件描述符就绪，就能通知用户态程序进行相应的读写操作；

没有文件描述符就绪时，会阻塞用户态程序，交出CPU；

**复用**是指一个线程处理多个文件描述符

常见的IO多路复用模型有**select/poll/epoll**



#include <sys/epoll.h>

int epoll_create(int size);//被弃用了



## epoll_create1()

```c++
int epoll_create1(int flags); 	// 创建一个epoll实例
// 参数: 选择属性
// 返回值: 成功时返回一个新的epoll实例的文件描述符, 失败返回-1并设置ERROR
```

**兴趣列表:**

​	这个列表存储了所有使用epoll_ct1( )注册到该epoll实例中的文件描述符, 还有为这些文件描述符指定的感兴趣的事件和用户数据. 这个兴趣列表的数据结构一般是红黑树

**就绪链表:**

​	这个列表存储了当前已经**就绪**（即有 I/O 事件发生）的文件描述符。

![image-20250729202603535](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729202603535.png)









## epoll_ct1()

```c++
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); // 用于操作epoll实例中注册的文件描述符
```

>**epfd:**
>
>这是上面那个函数epoll_create1()返回的epoll实例的文件描述符
>
>
>
>**op**: 
>
>EPOLL_CTL_ADD  添加
>
>EPOLL_CTL_MOD  修改
>
>EPOLL_CTL_DEL   删除
>
>
>
>**fd:**
>
>要操作的目标文件描述符
>
>
>
>**event:** 一个指向`struct epoll_event` 结构的指针, 用于指定与文件描述符关联的用户数据, 不用就是NULL
>
>下面是 struct epoll_event

```c++
struct epoll_event {
    uint32_t    events; /* Epoll events */
    epoll_data_t data;  /* User data variable */
};
```

> **uint32_t    events: **
>
>这是一个32位无符号整数, 用每一位的01表示事件是否发生, 因此我们可以用**位运算**符来操作它(|按位或or)
>
>将整型数据中的某一个位置为1，就表示该事件发生了
>
>EPOLLIN   读事件
>
>EPOLLOUT 写事件
>
>EPOLLET   边缘触发
>
>EPOLLIN | EPOLLOUT 同时启用
>
>
>
>**epoll_data_t    data:**
>
>这是一个联合体, 作用是将任意自定义的数据与特定的文件描述符结合
>
>typedef union epoll_data {
>    void    *ptr; // 指针，可以指向任何自定义数据结构
>    int     fd;   // 文件描述符本身
>    uint32_t u32; // 32位无符号整数
>    uint64_t u64; // 64位无符号整数
>} epoll_data_t;
>
>可以看到有一个void* ptr, 无类型指针可以指向任意类型的自定义类型, 只需要执行一次类型转换









## epoll_wait()

```c++
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 
// 等待epoll实例中注册的事件的发生, 这是事件循环的核心函数。程序调用 epoll_wait() 暂停执行，等待所关注的文件描述符上发生I/O事件。当事件发生时，它会唤醒并返回就绪的事件列表，让程序能够处理这些I/O操作。
```

>**epfd:**
>
>epoll_create1() 返回的 epoll 实例的文件描述符。
>
>
>
>**events:**
>
>一个指向 `struct epoll_event` 结构数组的指针, 在**用户态**定义的一个数组，当期待的事件发生, epoll_wait函数返回时，会从内核态就绪链表中将已经就绪的文件描述符信息全部拷贝到用户态的该数组中
>
>
>
>**maxevents:**
>
>events 数组能够存放的最大事件数量。这个值必须大于 0
>
>
>
>**timeout:**
>
>超时时间（毫秒）。可以设为负数表示一直阻塞知道事件发生 / 可以设为0表示立即结束函数返回 / 可以设为>0表示将阻塞最多多少毫秒
>
>
>
>**返回值:**
>
>成功时返回就绪事件数量, 超时返回0, 失败返回-1并设置ERROR













## select

![image-20250729161716579](C:\Users\18388\Desktop\Linux-Study\Linux网络编程\Linux网络编程.assets\image-20250729161716579.png)

>nfd: 文件描述符范围[0, nfds)
>
>readfds: 读事件集合
>
>writefds: 写事件集合
>
>exceptfds:
>
>timeout: 超时事件集合

|  1   |      |      |      |      |      |      |      |
| :--: | :--: | :--: | ---- | ---- | ---- | ---- | ---- |

