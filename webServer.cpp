#include "webServer.h"


webServer::webServer() {

}

void webServer::eventListen() {
    listenFd = socket(AF_INET, SOCK_STREAM, 0);  //创建监听套接字
    struct sockaddr_in serverAddr;
    serverAddr.sin_port = 8888;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(listenFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    assert (ret >= 0);
    ret = listen(listenFd, 128);       //监听
    assert( ret != 0);
    int epFd = epoll_create(1);   //  创建树
    ev.data.fd = listenFd;
    ev.events = EPOLLIN;   
    epoll_ctl(epFd, EPOLL_CTL_ADD, &ev);  // 给lfd上树
}   

bool webServer::dealWithClient() {
    
    struct sockaddr_in addr;
    socklen_t len =  sizeof(addr);
    int clientFd = accept(listenFd, (struct sockaddr*)&addr, &len);
    char ip[16] = "";
    cout<<"New Client Connect,ip = "<<inet_pton(AF_INET, addr.sin_addr.s_addr, ip, 16)<<"Port = "<<ntohs(addr.sin_port);
    // 给cfd上树
    ev.data.fd = clientFd;
    ev.events = EPOLLIN | EPOLLET;
}
void webServer::evenLoop() {
    bool stopServer = false;
    while (!stopServer) {
        int respNum = epoll_wait(epFd, evs, MAX_EVEVENT_SIZE, -1); //响应的数目
        if(respNum < 0)   assert();
        else if(respNum == 0)   continue;
        else {
            for (auto &ch : evs) {
                if (ch.data.fd == listenFd && ch.events & EPOLLIN) {



                }
            }



        } 




    }
    





}