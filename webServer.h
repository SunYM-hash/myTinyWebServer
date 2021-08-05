#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "./lock/lock.h"
#include "./threadPool/threadPool.h"
#include "./http/httpConn.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
using namespace std;

const int MAX_EVEVENT_SIZE = 10000;
class webServer {
public:
    webServer();
    ~webServer();
    void webServer_init();  //初始化
    void sqlPool();         //数据池
    void threadPool();      //线程池
    void eventListen();     //监听    
    void evenLoop();        //事件循环
    void fcntl();
    bool dealWithClient();
    bool dealWithSignal();
    bool dealWithRead();
    bool dealWithWrite();

private:
    httpConn *user;
    struct epoll_event ev, evs[MAX_EVEVENT_SIZE];
    int listenFd;
    int m_OPT_LINGER;
    
};

#endif