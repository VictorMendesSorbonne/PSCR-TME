#include "TCPServer.h"
#include <unistd.h>
#include <iostream>
using namespace pr;
void TCPServer::handleClient(pr::Socket scom) {
    int lu;
    read(scom.getFD(),&lu, sizeof(int));
    lu++;
    write(scom.getFD(),&lu, sizeof(int));
    scom.close();
}

bool TCPServer::startServer(int port) {
    ss = new ServerSocket(port);
    if(ss->isOpen()){
        while(1) {
            std::cout<<"en attente sur le port" << port << std::endl;
            Socket sc = ss->accept();
            if(sc.isOpen()){
                threads.emplace_back(&TCPServer::handleClient, this,sc);
            }
        }
        return true;
    }
    return false;
}

TCPServer::~TCPServer() {
    for(auto &t :threads){
        t.join();
    }
}