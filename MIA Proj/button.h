//button.h
//控制按键的函数.
//copyright @ MIA project
//version 1.0

#ifndef _BUTTON
#define _BUTTON

#include <arduino.h>

class button
{
private:
    int buttonPin[4];
    uint8_t buttonState;
    uint8_t lastButtonState;

public:
    //定义button脚，读取按键状态，初始时为INPUT_PULLUP
    button(int buttonOK,int buttonF1,int buttonF2,int buttonF3);
    //判断四个按键的状态是否按下，若按下按位返回1
    uint8_t buttonCheck(void);
    //定制版按钮查看函数
    uint8_t buttonPeek(void);
    ~button();
};

extern button button1;

#endif