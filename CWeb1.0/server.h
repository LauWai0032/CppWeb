#pragma once
#include <arpa/inet.h>
#include <sys/epoll.h>
int initListenFD(unsigned short port);
int epollRun(int lfd);
int acceptConn(int epfd, int lfd);

