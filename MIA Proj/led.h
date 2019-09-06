//led.h
//控制共阳极数码管的封装好的函数.
//copyright @ MIA project
//version 4.0

#ifndef _LED
#define _LED
#include<Arduino.h>
//#include <avr/pgmspace.h>

#define LEDUP 0
#define LEDDOWN 1

//描述显示数字时共阳极数码管亮灭情况的表
static const uint8_t numtab[10] = {
    0b00000011,
    0b10011111,
    0b00100101,
    0b00001101,
    0b10011001,
    0b01001001,
    0b01000001,
    0b00011111,
    0b00000001,
    0b00001001};
//舞动功能时共阳极数码管亮灭情况的表
static const int dirtab[2] = {
    0b11000101,
    0b00111001};
class led
{
private:
    //记录连接LED各个管脚的编号
    int ledPintab[8];

public:
    //led构造函数,传入a-g-dp的管脚编号
    led(int a, int b, int c, int d, int e, int f, int g, int dp);
    ~led();
    //显示数字,传入要显示的数字(0-9)
    void shownum(int);
    //舞动模式,0表示向上,1表示向下.2表示下一个
    void ledwave(int);
    //跑马灯模式,6表示全息灭,0-5指定开始位置,7表示顺时针下一个,8表示逆时针下一个
    void ledcycle(int);
    //位控led模式,1-8位分别控制a-g-dp,要求1代表点亮
    void ledshow(unsigned int);
    //0灭，1亮，2交替
    void ledDp(int);
};

#endif