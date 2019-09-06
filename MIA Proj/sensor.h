//sensor.h
//有关声音传感的相关函数.
//copyright @ MIA project
//version 3.0

#ifndef _SENSOR
#define _SENSOR

#include <arduino.h>

class voicesensor
{
private:
    //记录连接的脚
    int voicePinA;
    int voicePinD;

public:
    //测试有无声音,有返回非零,无返回零.
    int hasvoice(void);
    //初始化函数,传入连接的脚
    voicesensor(int pinA,int pinD);
    ~voicesensor();
};

extern voicesensor voicesensor1;
#endif