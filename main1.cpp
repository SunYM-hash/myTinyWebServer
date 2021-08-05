#include <sys/epoll.h>
#include "stdio.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include "threadPool/threadPool.h"
//#include "./wrap/wrap.h"

const int PORT = 8808;

void readClientRequest(struct epoll_event *ev, int epFd) {
    char buf[1024] = "";
    //int len = sizeof(buf);
    int n = read(ev->data.fd, buf, sizeof(buf));
    if (n < 0) {
        perror("");
        return;
    }
    else if (n == 0) {
        printf("client close\n");
        close(ev->data.fd);
        epoll_ctl(epFd, EPOLL_CTL_DEL, ev->data.fd, ev);
        return;
    }
    else {
        int ret = 0;
        while (ret = read(ev->data.fd, buf, sizeof(buf)) > 0) {
            printf("read ok\n");
        }
    }
}

int main1() {
    /*
    int listenFd = tcp4bind(PORT, NULL);  //绑定
    Listen(listenFd, 128);
    */
    int listenFd = socket(PF_INET, SOCK_STREAM, 0);
    if (listenFd < 0) perror("socket错误");
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = PORT;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    int ret = bind(listenFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (ret < 0) perror("bind错误");
    listen(listenFd, 128);
    int epFd = epoll_create(1); //创建树
    struct epoll_event ev, evs[1024];
    ev.data.fd = listenFd;   
    ev.events = EPOLLIN;
    epoll_ctl(epFd, EPOLL_CTL_ADD, listenFd, &ev);
    while (1) {
        int readyNum = epoll_wait(epFd, evs, 1024, -1);
        if (readyNum < 0) {
            perror("");
            break;
        }
        else {
            for (int i = 0; i < readyNum; i++) {
                // 判断是否是lfd
                if (evs[i].data.fd == listenFd && evs[i].events & EPOLLIN) {
                    struct sockaddr_in cliaddr;
                    char ip[16] = "";
                    socklen_t addrlen = sizeof(cliaddr);
                    int clientFd = accept(listenFd, (struct sockaddr*)&cliaddr, &addrlen);
                    printf("new client ip = %s,port = %d\n", 
                        inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16), ntohs(cliaddr.sin_port));
                    ev.events = EPOLLIN;    //cfd上树
                    ev.data.fd = clientFd;
                    epoll_ctl(epFd, EPOLLIN, clientFd, &ev);
                }
                else if (evs[i].events & EPOLLIN) {
                    readClientRequest(&evs[i], epFd);

                }
            }
        }
    }

    return 0;
}