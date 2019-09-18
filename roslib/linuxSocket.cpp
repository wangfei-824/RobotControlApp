//
//  linuxSocket.cpp
//  testSocket
//
//  Created by Lucky on 19/8/7.
//  Copyright © 2019年 Felix. All rights reserved.
//

#include "linuxSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <errno.h>

#include <string>
#include <iostream>

#define DEFAULT_PORT "11411"

using std::string;
using std::cerr;
using std::endl;


class linuxSocketImpl {
    
public:
    
    linuxSocketImpl() : _socket(-1), _is_connected(false) { }
    
    void init(const string & server_hostname) {
        _is_connected = false;
        
        if(-1 == _socket) {
            _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if(-1 == _socket) {
                std::cerr << "Could not create socket " << std::endl;
                return;
            }
        }
        
        // split off the port number if given
        size_t c = server_hostname.find_last_of (':');
        std::string host = server_hostname.substr (0, c);
        std::string port = (c == std::string::npos) ? DEFAULT_PORT : server_hostname.substr (c + 1);
        
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(host.c_str());
        server_addr.sin_port = htons(atoi(port.c_str()));
        
        unsigned long mode = 0;
        ioctl(_socket, FIONBIO, &mode);
        
        std::cout << "connecting..." << std::endl;
        int ret = connect(_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
        
        if(ret) {
            std::cerr << "Could not connect to server " << server_hostname << std::endl;
            close(_socket);
            _socket = -1;
            _is_connected = false;
            return;
        }
        
        _is_connected = true;
        
//        int error=-1, len;
//        timeval tm;
//        fd_set set;
//        unsigned long mode = 1;
//        ioctl(_socket, FIONBIO, &mode);
//        
//        if(-1 == connect(_socket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr))) {
//            tm.tv_sec = 2;
//            tm.tv_usec = 0;
//            FD_ZERO(&set);
//            FD_SET(_socket, &set);
//            if(select(_socket+1, NULL, &set, NULL, &tm) > 0) {
//                getsockopt(_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
//                if (0 == error) {
//                    _is_connected = true;
//                } else {
//                    _is_connected = false;
//                }
//            } else {
//                _is_connected = false;
//            }
//        } else {
//            _is_connected = true;
//        }
//        
//        mode = 0;
//        ioctl(_socket, FIONBIO, &mode);
//        
//        if(!_is_connected) {
//            std::cerr << "Could not connect to server " << server_hostname << std::endl;
//            close(_socket);
//            _socket = -1;
//            return;
//        }
        
    }
    
    bool isConnected() {
        return _is_connected;
    }
    
    int read () {
        char data;
        ssize_t result = recv(_socket, &data, 1, MSG_DONTWAIT);
        
        if(result < 0) {
//            std::cerr << "Failed to receive data from server " << std::endl;
            return -1;
        }
        else if (result == 0) {
            std::cerr << "Connection to server closed" << std::endl;
            close(_socket);
            _socket = -1;
            _is_connected = false;
            return -1;
        }
        return (unsigned char) data;
    }
    
    void write (const unsigned char *data, int length) {
        ssize_t result = send(_socket, (const char *) data, length, 0);
        
        if(result < 0) {
            close(_socket);
            _socket = -1;
            _is_connected = false;
            std::cerr << "Send failed" << std::endl;
        }
    }
    
    unsigned long time () {
        struct timeval tv;
        struct timezone tz;
        gettimeofday(&tv, &tz);
        
        unsigned long millis = tv.tv_sec * 1000 + tv.tv_usec / 1000;
        return millis;
    }
    
    void closeSocket() {
        close(_socket);
        _socket = -1;
        _is_connected = false;
    }
    
private:
    int _socket;
    bool _is_connected;
};

linuxSocket::linuxSocket () {
    impl = new linuxSocketImpl ();
}

void linuxSocket::init (char *server_hostname) {
    impl->init (server_hostname);
}

bool linuxSocket::isConnected () {
    return impl->isConnected ();
}

int linuxSocket::read () {
    return impl->read ();
}

void linuxSocket::write (const unsigned char *data, int length) {
    impl->write (data, length);
}

unsigned long linuxSocket::time () {
    return impl->time ();
}

void linuxSocket::close() {
    impl->closeSocket ();
}
