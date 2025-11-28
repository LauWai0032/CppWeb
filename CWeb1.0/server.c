#include "server.h"

/**
 * @brief 初始化服务器监听套接字
 * 
 * 该函数完成TCP服务器监听套接字的创建、选项设置、地址绑定和监听操作，
 * 最终返回可用的监听文件描述符，失败时返回-1并打印错误信息。
 * 
 * @param port 服务器要监听的端口号（主机字节序）
 * @return int 成功返回监听文件描述符（lfd），失败返回-1
 */
int initListenFD(unsigned short port)
{   
    // 1. 创建TCP套接字（文件描述符）
    // AF_INET：使用IPv4协议族
    // SOCK_STREAM：面向连接的TCP协议
    // 0：默认协议（TCP）
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {          // 套接字创建失败
        perror("socket");     // 打印错误原因（errno对应的描述）
        return -1;            // 失败返回-1
    }

    // 2. 设置套接字选项：允许端口复用
    // 解决服务器重启时"Address already in use"的问题（TIME_WAIT状态占用端口）
    // SOL_SOCKET：操作套接字层选项
    // SO_REUSEADDR：允许复用本地地址和端口
    // &(int){1}：临时创建值为1的int变量，启用该选项
    // sizeof(int)：选项值的长度
    int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
    if (ret == -1) {          // 选项设置失败
        perror("setsockopt"); // 打印错误信息
        close(lfd);           // 关闭已创建的套接字，避免资源泄漏
        return -1;            // 失败返回-1
    }

    // 3. 初始化服务器地址结构体
    struct sockaddr_in serv_addr;  // IPv4地址结构体
    serv_addr.sin_family = AF_INET; // 地址族：IPv4
    // 端口号转换：主机字节序（host）转网络字节序（network），网络传输必须用网络字节序
    serv_addr.sin_port = htons(port);
    // 监听所有网卡的IP地址（INADDR_ANY = 0.0.0.0），接收任意客户端的连接
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // 4. 绑定套接字到指定的IP和端口
    // (struct sockaddr*)&serv_addr：通用地址结构体类型转换（兼容要求）
    // sizeof serv_addr：地址结构体的长度
    ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof serv_addr);
    if (ret == -1) {          // 绑定失败
        perror("bind");       // 打印错误信息
        close(lfd);           // 关闭套接字释放资源
        return -1;            // 失败返回-1
    }

    // 5. 将套接字设置为监听模式，开始监听客户端连接
    // 128：监听队列的最大长度（未处理的连接数上限），受系统限制（如/proc/sys/net/core/somaxconn）
    ret = listen(lfd, 128);
    if (ret == -1) {          // 监听失败
        perror("listen");     // 打印错误信息
        close(lfd);           // 关闭套接字释放资源
        return -1;            // 失败返回-1
    }

    // 所有操作成功，返回监听文件描述符
    return lfd;
}

int epollRun(int lfd)
{
    int epfd = epoll_create(1);
    if(epfd == -1) {
        perror("epoll_create");
        return -1;
    }
    struct epoll_event event;
    event.data.fd = lfd;
    event.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &event);
    if(ret == -1) {
        perror("epoll_ctl");
        return -1;
    }
    while(1) {
        struct epoll_event events[1024];
        int nready = epoll_wait(epfd, events, sizeof events/sizeof(struct epoll_event), -1);
        if(nready == -1) {
            perror("epoll_wait");
            return -1;
        }
        for(int i = 0; i < nready; ++i) {
            if(events[i].data.fd == lfd) {
                // Handle new connection
                
            } else {
                // Handle data from connected clients
            }
        }
    }
    return 0;
}
