//
//  linuxSocket.hpp
//  testSocket
//
//  Created by Lucky on 19/8/7.
//  Copyright © 2019年 Felix. All rights reserved.
//

#ifndef linuxSocket_h
#define linuxSocket_h

// forward declaration of the implementation class
// this class is defined in the implementation file to abstract all of the
// windows specific crud. It gets in the way of the ROS libraries.
class linuxSocketImpl;

class linuxSocket
{
public:
    linuxSocket ();
    
    void init (char *server_hostname);
    
    bool isConnected();
    
    int read ();
    
    void write (const unsigned char *data, int length);
    
    unsigned long time ();
    
    void close();
    
private:
    linuxSocketImpl * impl;
};


#endif /* linuxSocket_hpp */
