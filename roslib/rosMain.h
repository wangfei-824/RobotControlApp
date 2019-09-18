//
//  rosMain.h
//  testSocket
//
//  Created by Lucky on 19/8/7.
//  Copyright © 2019年 Felix. All rights reserved.
//

#ifndef rosMain_h
#define rosMain_h

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    void rosMainInit(char *server_hostname);
    
    void rosMainClose();
    
    bool rosMainIsConnected();
    
    void rosMainLoop();
    
    void rosMainSendCmd(int cmd);
    
    void rosMainSendCmdVelocity(double linear, double angular);
    
    void rosMainSetPoseCallback(void (*PoseCallback)(double, double, double, double, double, double));
        
#ifdef __cplusplus
}
#endif
#endif /* rosMain_h */
