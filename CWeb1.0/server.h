#pragma once
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
int initListenFD(unsigned short port);
int epollRun(int lfd);
int acceptConn(int lfd, int epfd);
