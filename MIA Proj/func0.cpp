//func0.cpp
//功能0
//copyright @ MIA project
//version 2.0

#include "showu8g2.h"
#include <LiquidCrystal.h>
#include "button.h"
#include "MPU6050_tockn.h"
#include <Wire.h>
#include "fft_c.h"
#include "led.h"
#include "eeprom.h"
#include "miaTool.h"
//定义难度
#define DIFFICULTY_EASY 20
//定义难度
#define DIFFICULTY_HARD 10

extern led led1;
extern LiquidCrystal lcd1;
extern button button1;

extern U8G2_ST7920_128X64_1_8080 u8g2;
void showEnemy(unsigned int n, uint8_t x, uint8_t y);
void showTarget(uint8_t x, uint8_t y);
unsigned int showEnemyTone(uint8_t enemyStatus, uint8_t i);

unsigned int showEnemyTone(uint8_t enemyStatus, uint8_t i)
{
    if (bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 1) == 0 && bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 0) == 0)
        return 0;
    if (bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 1) == 0 && bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 0) == 1)
        return 1;
    if (bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 1) == 1 && bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 0) == 0)
        return 2;
    if (bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 1) == 1 && bitRead(((enemyStatus >> (i * 2)) & 0b00000011), 0) == 1)
        return 3;
}

void func0()
{
    //变量定义，可以继续添加
    //led显示计数器
    unsigned int ledCount = 0;
    //大招次数记录
    unsigned int dazhaoCount = 0;
    //频率记录
    double tempTune = 0;
    //灵敏度,取值1-3,数值越大灵敏度越高
    uint8_t sensibility = 2;
    //初始水平位置
    double foucusLR = 0;
    //初始仰角
    double foucusUD = 0;
    //敌人人口
    double enemyBorn = 0;
    //关于敌人生成算法的时间记录
    unsigned int tyjTimeCount = 0;
    //敌人数目记录
    uint8_t enemyCount = 0;
    //瞄准头横纵坐标
    uint8_t focusX, focusY;
    uint8_t oldFocusX, oldFocusY;
    //敌人状态，1||2||3：活；0：死
    uint8_t enemyStatus;
    uint8_t oldEnemyStatus;
    //敌人出生时间(毫秒)
    unsigned long enemyBirthday[4];
    //游戏开始时间(毫秒)
    unsigned long gameStartTime;
    //暂停开始时间(毫秒)
    unsigned long stopTime;
    //敌人音调，每位敌人分配三个，0表示没有敌人或此音已按
    uint8_t enemyTone[12];
    //分数
    unsigned int score;
    //高难度最高分
    unsigned int hardScoreMax;
    //低难度最高分
    unsigned int easyScoreMax;
    //难度
    uint8_t difficulty;
    //按钮状态
    uint8_t tempStatus;
    uint8_t tempStatus1;
    //开始界面
    int flag = -5;

    //开场动画,十字线
    u8g2.firstPage();
    do
    {

        u8g2.drawHLine(0, 32, 128);
        u8g2.drawVLine(64, 0, 64);
    } while (u8g2.nextPage());
    lcd1.clear();
    lcd1.noBlink();
    lcd1.setCursor(8, 0);
    lcd1.print(F("AOAS"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("ambush on all sides "));
    lcd1.setCursor(0, 2);
    lcd1.print(F("initing keep still"));
    lcd1.setCursor(2, 3);
    lcd1.print(F("version 2.0 By MIA"));

    //mpu6050模块初始化
    Wire.begin();
    mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
    delay(1000);

func00a:

    //变量初始化
    focusX = 0;
    focusY = 0;
    enemyStatus = 0;
    for (int i = 0; i < 4; i++)
        enemyBirthday[i] = 0;
    for (int i = 0; i < 12; i++)
        enemyTone[i] = 0;
    score = 0;
    ledCount = 0;
    dazhaoCount = 0;
    tempTune = 0;
    sensibility = 2;
    enemyBorn = 0;
    tyjTimeCount = 0;
    enemyCount = 0;
    mpu6050.update();
    foucusLR = mpu6050.getAngleZ();
    foucusUD = mpu6050.getAccAngleY();
    //读简单难度最高分
    long startAddr = EEPROM.length() - 100;
    EEPROM.get(startAddr + 0, easyScoreMax);
    //读困难难度最高分
    EEPROM.get(startAddr + 2, hardScoreMax);

    lcd1.clear();
    lcd1.setCursor(8, 0);
    lcd1.print(F("AOAS"));
    lcd1.setCursor(8, 1);
    lcd1.print(F("PLAY"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("   easy      hard   "));
    lcd1.setCursor(7, 3);
    lcd1.print(F("Record       "));
    lcd1.setCursor(7 + flag, 2);
    lcd1.blink();
    while (1)
    {

        tempStatus = button1.buttonCheck();
        //back退出
        if (bitRead(tempStatus, 3))
        {
            goto func00end;
        }
        //F1选择难度
        if (bitRead(tempStatus, 1))
        {
            flag = -flag;

            lcd1.setCursor(7 + flag, 2);
        }
        //F2选择最高分
        if (bitRead(tempStatus, 2))
        {
            lcd1.setCursor(3, 16);
            while (1)
            {
                tempStatus = button1.buttonCheck();
                //再按F1，光标跳回选择难度
                if (bitRead(tempStatus, 1))
                    goto func00a;
                //进入查询最高分界面
                if (bitRead(tempStatus, 0))
                {
                    lcd1.clear();
                    lcd1.noBlink();
                    lcd1.setCursor(7, 0);
                    lcd1.print(F("Record"));
                    lcd1.setCursor(6, 1);
                    lcd1.print(F("easy:"));
                    lcd1.setCursor(12, 1);
                    lcd1.print(easyScoreMax);
                    lcd1.setCursor(6, 2);
                    lcd1.print(F("hard:"));
                    lcd1.setCursor(12, 2);
                    lcd1.print(hardScoreMax);
                }

                //back退出
                if (bitRead(tempStatus, 3))
                    goto func00a;
            }
        }
        if (bitRead(tempStatus, 0))
        {
            //hard模式
            if (flag == 5)
            {
                difficulty = DIFFICULTY_HARD;
            }
            //easy模式
            if (flag == -5)
            {
                difficulty = DIFFICULTY_EASY;
            }
            //退出循环
            break;
        }
    }
    //安全性初始化
    lcd1.noBlink();
    gameStartTime = millis();
    lcd1.clear();

    //游戏中
    while (1)
    {

        //生成敌人
        if (tyjTimeCount != ((millis() - gameStartTime) / 1000))
        { //进入新的一秒
            tyjTimeCount = ((millis() - gameStartTime) / 1000);
            //根据公式增加敌人人口
            enemyBorn += sqrt((millis() - gameStartTime) / 1000) * (0.1 / difficulty) + 0.2;
        }
        //如果有人口产生敌人且场上有位置
        if (enemyBorn > 1 && enemyCount < 4)
        {
            //初始化随机函数并产生随机数
            randomSeed(millis());
            uint8_t tempRandNum = random() % 4;

            for (int i = 0; i < 4; i++)
            {
                if (((enemyStatus >> (tempRandNum * 2)) & 0b00000011) == 0)
                {
                    //更新数据记录
                    enemyBirthday[tempRandNum] = millis();
                    enemyBorn -= 1;
                    enemyCount++;
                    //决定敌人类型并更新enenmyTone
                    unsigned int fuckTemp = (random() % 60 + 15 * (4 - enemyCount) + sqrt((millis() - gameStartTime) / 1000) * 64 / difficulty) / 40;
                    uint8_t tempuint8 = constrain(fuckTemp, 1, 3);
                    enemyStatus |= (tempuint8 << (tempRandNum * 2));
                    for (uint8_t j = 0; j < tempuint8; j++)
                    {
                        enemyTone[tempRandNum * 3 + j] = (random() % 7) + 4;
                    }
                    break;
                }
                tempRandNum = (tempRandNum + i + 1) % 4;
            }
        }

        //Focus更新
        mpu6050.update();
        focusX = map(constrain((mpu6050.getAngleZ() - foucusLR), -60 + sensibility * 15, 60 - sensibility * 15), -60 + sensibility * 15, 60 - sensibility * 15, 0, 127);
        focusY = map(constrain((mpu6050.getAccAngleY() - foucusUD), -36 + sensibility * 4, 36 - sensibility * 4), -36 + sensibility * 4, 36 - sensibility * 4, 0, 63);

        //刷屏
        lcd1.noBlink();
        lcd1.clear();
        lcd1.setCursor(7, 1);
        lcd1.print(F("Time:"));
        lcd1.print((millis() - gameStartTime) / 1000);
        lcd1.setCursor(6, 2);
        lcd1.print(F("Score:"));
        lcd1.print(score);
        //
        for (uint8_t i = 0; i < 4; i++)
        {
            if (enemyBirthday[i] > 0)
            {
                lcd1.setCursor(((i + 2) % 2 == 0 ? 0 : 18), (i < 2 ? 1 : 2));
                lcd1.print(difficulty - (millis() - enemyBirthday[i]) / 1000);
                //lcd1.print(" ");
                for (uint8_t j = 0; j < ((enemyStatus >> (i * 2)) & 0b00000011); j++)
                {
                    lcd1.setCursor((((i + 2) % 2 == 0 ? 0 : 12) + j * 3), (i < 2 ? 0 : 3));
                    if ((enemyTone[i * 3 + j]) == 0)
                    {
                        lcd1.print(F("."));
                    }
                    else
                    {
                        lcd1.print((enemyTone[i * 3 + j] - 3));
                    }
                }
            }
        }

        //光标闪烁

        lcd1.setCursor((focusX < 64 ? 1 : 17), (focusY < 32 ? 1 : 2));
        lcd1.blink();

        //B屏画敌人
        if ((abs(oldFocusX - focusX)) || (abs(oldFocusY - focusY)) || (oldEnemyStatus != enemyStatus))
        {
            u8g2.firstPage();
            do
            {
                //十字线
                u8g2.drawHLine(0, 32, 128);
                u8g2.drawVLine(64, 0, 64);
                //每个区域的敌人
                for (uint8_t i = 0; i < 4; i++)
                {
                    if (enemyBirthday[i] > 0)
                    {
                        showEnemy(((enemyStatus >> (i * 2)) & 0b00000011), ((i + 2) % 2 == 0 ? 25 : 89), (i < 2 ? 4 : 36));
                    }
                }
                //准头
                showTarget(focusX, focusY);
                oldFocusX = focusX;
                oldFocusY = focusY;
                oldEnemyStatus = enemyStatus;

            } while (u8g2.nextPage());
        }

        //击杀、大招
        tempTune = fuckingCompute();
        //大招触发判断
        if (abs(tempTune - 523) < 10)
        {
            if (dazhaoCount)
            {
                dazhaoCount--;
                for (uint8_t i = 0; i < 4; i++)
                {
                    if (((enemyStatus >> (i * 2)) & 0b00000011) != 0)
                    {
                        score++;
                        enemyStatus &= (~(0b00000011 << (i * 2)));
                        enemyBirthday[i] = 0;
                        for (uint8_t j = 0; j < 3; j++)
                        {
                            enemyTone[i * 3 + j] = 0;
                        }
                        enemyCount--;
                    }
                }
            }
        }
        else
        {
            //正常击杀模式
            uint8_t tempPlace = 0;
            if (focusX > 64)
            {
                tempPlace += 1;
            }
            if (focusY > 32)
            {
                tempPlace += 2;
            }
            if (((enemyStatus >> (tempPlace * 2)) & 0b00000011) != 0)
            {
                uint8_t i = 0;
                while (enemyTone[tempPlace * 3 + i] == 0)
                {
                    i++;
                }
                if (abs(frequencyTab[enemyTone[tempPlace * 3 + i]] - tempTune) < 10)
                {
                    enemyTone[tempPlace * 3 + i] = 0;
                    i++;
                }
                if (i >= ((enemyStatus >> (tempPlace * 2)) & 0b00000011))
                {
                    score++;
                    enemyStatus &= (~(0b00000011 << (tempPlace * 2)));
                    enemyCount--;
                    enemyBirthday[tempPlace] = 0;
                }
            }
        }

        //led大招模块
        if ((score / 10) > ledCount)
        {
            ledCount = score / 10;
            dazhaoCount++;
        }
        if (dazhaoCount)
        {
            led1.shownum(0);
        }
        else
        {
            led1.shownum(constrain(10 - (score % 10), 0, 9));
        }
        //死亡判断
        for (uint8_t i = 0; i < 4; i++)
        {
            if ((millis() - enemyBirthday[i]) > difficulty * 1000 && enemyBirthday[i] > 0)
                goto func00end;
        }

        //按键、暂停
        tempStatus1 = button1.buttonCheck();
        if (bitRead(tempStatus1, 0))
        {
            stopTime = millis();

        //暂停界面
        func0stop1:
            lcd1.noBlink();
            lcd1.clear();
            lcd1.setCursor(7, 0);
            lcd1.print(F("Pause"));
            lcd1.setCursor(0, 1);
            lcd1.print(F("F1:Adjust"));
            lcd1.setCursor(0, 2);
            lcd1.print(F("F2:Sensibility"));
            u8g2.firstPage();
            do
            {
                u8g2.setFont(u8g2_font_ncenB14_tr);
                u8g2.drawStr(35, 40, "Pause");
            } while (u8g2.nextPage());

            while (1)
            {
                tempStatus = button1.buttonCheck();
                if (bitRead(tempStatus, 0))
                {
                    break;
                }
                if (bitRead(tempStatus, 1))
                {
                    //校准
                    mpu6050.update();
                    foucusLR = mpu6050.getAngleZ();
                    foucusUD = mpu6050.getAccAngleY();
                    lcd1.clear();
                    lcd1.setCursor(2, 2);
                    lcd1.print(F("Adjust succeed"));
                    delay(500);
                    break;
                }
                //灵敏度调节
                if (bitRead(tempStatus, 2))
                {
                    lcd1.clear();
                    lcd1.setCursor(4, 0);
                    lcd1.print(F("Sensibility"));
                    lcd1.setCursor(0, 1);
                    lcd1.print(F("F1:Low"));
                    lcd1.setCursor(0, 2);
                    lcd1.print(F("F2:Medium"));
                    lcd1.setCursor(0, 3);
                    lcd1.print(F("F3:High"));
                    while (1)
                    {
                        tempStatus = button1.buttonCheck();
                        if (bitRead(tempStatus, 1))
                        {
                            sensibility = 1;
                            break;
                        }
                        if (bitRead(tempStatus, 2))
                        {
                            sensibility = 2;
                            break;
                        }
                        if (bitRead(tempStatus, 3))
                        {
                            sensibility = 3;
                            break;
                        }
                    }
                    lcd1.clear();
                    lcd1.setCursor(2, 2);
                    lcd1.print(F("Adjust succeed"));
                    delay(500);
                    break;
                }
            }
            // 时间纠正
            stopTime = millis() - stopTime;
            gameStartTime += stopTime;
            for (uint8_t j = 0; j < 4; j++)
            {
                if (enemyBirthday[j] > 0)
                {
                    enemyBirthday[j] += stopTime;
                }
            }
        }
        //退出游戏
        if (bitRead(tempStatus1, 3))
            break;
    }

    //收尾处理
func00end:
    //死亡界面
    if ((score <= easyScoreMax && difficulty == DIFFICULTY_EASY) || (score <= hardScoreMax && difficulty == DIFFICULTY_HARD))
    {
        lcd1.clear();
        lcd1.setCursor(5, 0);
        lcd1.print(F("Game Over"));
        lcd1.setCursor(0, 1);
        lcd1.print(F("Your score:"));
        lcd1.setCursor(11, 1);
        lcd1.print(score);
        lcd1.setCursor(0, 2);
        lcd1.print(F("ok to continue"));
        lcd1.setCursor(0, 3);
        lcd1.print(F("F3:back"));

        u8g2.firstPage();
        do
        {
            u8g2.setFont(u8g2_font_ncenB12_tr);
            u8g2.drawStr(5, 50, "GAME OVER");

        } while (u8g2.nextPage());
        while (1)
        {
            tempStatus = button1.buttonCheck();
            //back退出
            if (bitRead(tempStatus, 3))
                break;
            //ok 则回到选择难度界面(再来一局)
            if (bitRead(tempStatus, 0))
            {
                u8g2.firstPage();
                do
                {
                    u8g2.drawHLine(0, 32, 128);
                    u8g2.drawVLine(64, 0, 64);
                } while (u8g2.nextPage());
                goto func00a;
            }
        }
    }
    if ((score > easyScoreMax && difficulty == DIFFICULTY_EASY) || (score > hardScoreMax && difficulty == DIFFICULTY_HARD))
    {
        //记录新纪录

        long startAddr = EEPROM.length() - 100;
        if (difficulty == DIFFICULTY_EASY)
        {
            EEPROM.put(startAddr + 0, score);
        }
        else
        {
            EEPROM.put(startAddr + 2, score);
        }

        lcd1.clear();
        lcd1.setCursor(3, 0);
        lcd1.print(F("New Record!"));
        lcd1.setCursor(0, 1);
        lcd1.print(F("Your score:"));
        lcd1.setCursor(11, 1);
        lcd1.print(score);
        lcd1.setCursor(0, 2);
        lcd1.print(F("ok to continue"));
        lcd1.setCursor(0, 3);
        lcd1.print(F("back to exit"));
        u8g2.firstPage();
        do
        {
            u8g2.setFont(u8g2_font_ncenB10_tr);
            u8g2.drawStr(1, 47, "Congratulations!");

        } while (u8g2.nextPage());
        while (1)
        {
            tempStatus = button1.buttonCheck();
            //back退出
            if (bitRead(tempStatus, 3))
                break;
            //ok 则回到选择难度界面(再来一局)
            if (bitRead(tempStatus, 0))
            {
                u8g2.firstPage();
                do
                {
                    u8g2.drawHLine(0, 32, 128);
                    u8g2.drawVLine(64, 0, 64);
                } while (u8g2.nextPage());
                goto func00a;
            }
        }
    }
}
