//showu8g2.h
//u8g2屏中显示的字符函数
//copyright @ MIA project
//version 1.0

#ifndef _SHOWU8G2
#define _SHOWU8G2
#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

extern U8G2_ST7920_128X64_1_8080 u8g2;

//显示1、2、3、4、5、6、7、i八中音调
void showtone(int Num);
//显示菜单封面
void showMenuCover(int Num);
//显示敌人，n为敌人生命数/音调数(1或2或3)，x、y为敌人头部中心坐标
void showEnemy(unsigned int n, uint8_t x, uint8_t y);
//显示准星，x、y为准星中心
void showTarget(uint8_t x, uint8_t y);
//显示祝贺图片
void showCongratulation();
//显示箭头，direction中1上0下
void showBArrow(uint8_t direction, int col);
//展现音符（全音符4，二分音符3，四分音符2，八分音符1，十六分音符0),x、y为基准坐标
void showSpecificTone(uint8_t result,uint8_t x, uint8_t y);
//展现音符，Num(0代表十六分，1代表八分，2代表四分，3代表二分，4代表全)
void showBigTone(int Num,uint8_t x, uint8_t y);
//展现音符，Num(后五位从右往左分别代表十六分、八分、四分、二分、全)
void showBigTonea(uint8_t Num, uint8_t x, uint8_t y);
#endif