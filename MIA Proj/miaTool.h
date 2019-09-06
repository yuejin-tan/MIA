//miaTool.h
//工具性模块集合
//copyright @ MIA project
//version 4.0

#ifndef _MIATOOL
#define _MIATOOL

#include <arduino.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include "eepromMgr.h"
#include <avr/pgmspace.h>
#include "button.h"

//定义音频输出脚
#define speakerPin A10
//定义菜单选择传感器脚
#define menuPin A8

extern LiquidCrystal lcd1;
extern eepromMgr eepromMgr1;

//音乐频率映射关系表
extern const int frequencyTab[15];

//随机歌曲填充函数
void randomSong(int type);

//嵌入型文件浏览器
int exploreFile00(void);

//嵌入型文件浏览器辅助函数
void exploreShow(int line);

//蓝牙包装类
class bluetooth
{
private:
    int bluetoothPin;

public:
    int BTLinkStatus();
    bluetooth(int bluetoothPin0);
    ~bluetooth();
};

extern bluetooth bluetooth1;

//主菜单bgm实现模块的包装类
class bgm
{
private:
    unsigned int bgmPointer,bgmLenth;
    unsigned long tempTime;
    unsigned long tempTime2;
    unsigned int cnt1;
    unsigned long duration;

public:
    bgm();
    ~bgm();
    void startBgm(void);
    void nextBgm(void);
    void stopBgm(void);
};

//超级用户模式实现函数
void superUser(void);

#endif