//MIA.ino
//主程序
//copyright @ MIA project
//version 4.0

#include "LiquidCrystal.h"
#include "arduino.h"
#include "miaTool.h"
#include "led.h"
#include "button.h"
#include "eepromMgr.h"
#include "sensor.h"
#include "showu8g2.h"
#include "MPU6050_tockn.h"

//lcd1的接脚
const int rs = A2, en = A3, d4 = A4, d5 = A5, d6 = A6, d7 = A7;
//定义lcd1
//LiquidCrystal lcd1(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);
LiquidCrystal lcd1(rs, en, d4, d5, d6, d7);
//定义button1
button button1(43, 45, 47, 49);
//定义led
led led1(24, 26, 25, 27, 29, 22, 28, 23);
//定义存储管理器;
eepromMgr eepromMgr1;
//音量传感器
voicesensor voicesensor1(A0, A1);
//12864显示屏
U8G2_ST7920_128X64_1_8080 u8g2(U8G2_R2, 34, 35, 36, 37, 38, 39, 40, 41,
                               /*enable=*/32, /*cs=*/U8X8_PIN_NONE, /*dc/rs=*/31, /*reset=*/33);
//加速度传感器
MPU6050 mpu6050(Wire);
//蓝牙模块
bluetooth bluetooth1(30);
//bgm响应模块
bgm bgm1;
//舵机模块
Servo servo1;
//主菜单表
char menuTab[7][21] = {{"                    "}, {"1.Games             "}, {"2.single tone       "}, {"3.Rhythm            "}, {"4.MIA-LINK          "}, {"5.file System       "}, {"                    "}};

//主菜单显示函数
void showMenu(int line, int place)
{
    lcd1.setCursor(0, place);
    lcd1.print(menuTab[line]);
}

//声明功能函数
#define MENUNUM 5
void func0();
void func1();
void func2();
void func3();
void func4();
//初始化开始菜单选择函数指针列表
void (*menuPointer[MENUNUM])() = {
    func0, func1, func2, func3, func4};

void setup()
{
    int passwordCnt = 0;
    int isright = 1;
    uint8_t tempResult;

    //初始化屏幕
    u8g2.begin();
    lcd1.begin(20, 4);
    //初始化菜单选择引脚
    pinMode(menuPin, INPUT);
    //初始化音频输出脚
    pinMode(speakerPin, OUTPUT);
    //初始化舵机
    pinMode(2, OUTPUT);
    servo1.attach(2);
    servo1.write(90);

    Serial.begin(9600);
    byte debug;
    for (uint8_t i = 0; i < 64; i++)
    {
        debug = EEPROM.read(i);
        Serial.println(debug);
    }
    Serial.flush();
    Serial.end();

superUser2:

    u8g2.firstPage();
    do
    {
        u8g2.setFont(u8g2_font_ncenB24_tr);
        u8g2.drawStr(30, 45, "MIA");
    } while (u8g2.nextPage());

    while (1)
    {
        //初始化密码存储
        for (uint8_t i = 0; i < 10; i++)
        {
            nameBuff[i] = 16;
        }
        for (uint8_t i = 0; i < 16; i++)
        {
            pubBuff[i] = 16;
        }
        eepromMgr1.getPassword();
        isright = 1;
        passwordCnt = 0;

        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print(F("Login"));
        lcd1.setCursor(0, 1);
        lcd1.print(F("Enter password"));
        lcd1.setCursor(0, 3);
        lcd1.print(F("OK"));
        lcd1.setCursor(0, 2);
        lcd1.blink();
        while (1)
        {
            tempResult = button1.buttonCheck();
            if (bitRead(tempResult, 1))
            {
                nameBuff[passwordCnt] = 0;
                passwordCnt++;
                lcd1.print(F("0"));
            }
            if (bitRead(tempResult, 2))
            {
                nameBuff[passwordCnt] = 1;
                passwordCnt++;
                lcd1.print(F("1"));
            }
            if (bitRead(tempResult, 3))
            {
                nameBuff[passwordCnt] = 2;
                passwordCnt++;
                lcd1.print(F("2"));
            }
            if (bitRead(tempResult, 0) || passwordCnt == 10)
            {
                break;
            }
        }
        //superUser 检测
        for (int i = 0; i < 10; i++)
        {
            if (nameBuff[i] != 1)
                isright = 0;
        }
        if (isright == 1)
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                nameBuff[i] = 16;
            }
            superUser();
            goto superUser2;
        }
        isright = 1;

        uint8_t fuckuint8 = 0;
        while (pubBuff[fuckuint8] < 3)
        {
            fuckuint8++;
        }
        fuckuint8 = constrain(fuckuint8, 1, 10);

        for (int i = 0; i < constrain(passwordCnt, fuckuint8, 10); i++)
        {

            if (pubBuff[i] != nameBuff[i])
                isright = 0;
        }
        if (isright == 0)
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("Error"));
            lcd1.setCursor(0, 1);
            lcd1.print(F("Wrong Password"));
            lcd1.setCursor(0, 3);
            lcd1.print(F("Try Again"));
            while (button1.buttonCheck() != 0b00000001)
            {
            }
        }
        if (isright == 1)
        {
            break;
        }
    }

    lcd1.noBlink();
}

void loop()
{
    uint8_t btnLine;
    uint8_t mainLine;
    uint8_t sensorLine;
    uint8_t oldMainLine;
    uint8_t oldSensorLine;
    uint8_t tempStatus;

    mainLine = 1;
    btnLine = 1;
    sensorLine = map(constrain(analogRead(menuPin), 100, 900), 100, 900, 1, 5);
    oldMainLine = 0;
    oldSensorLine = sensorLine;
    tempStatus = 0;

    bgm1.startBgm();

    led1.ledshow(0);
    lcd1.clear();
    lcd1.setCursor(8, 0);
    lcd1.print(F("MENU"));

    do
    {
        //更新滚动区域
        if (mainLine != oldMainLine)
        {
            lcd1.noBlink();
            for (int i = 0; i < 3; i++)
            {
                showMenu(mainLine + i - 1, 1 + i);
            }
            showMenuCover(mainLine);
            led1.shownum(mainLine);
            tone(speakerPin, 4096, 100);
            lcd1.setCursor(19, 2);
            lcd1.blink();
            oldMainLine = mainLine;
            //更新按键及btnLine
        }
        tempStatus = button1.buttonCheck();
        if (bitRead(tempStatus, 1))
        {
            if (btnLine < 5)
            {
                btnLine++;
            }
            mainLine = btnLine;
        }
        if (bitRead(tempStatus, 2))
        {
            if (btnLine > 1)
            {
                btnLine--;
            }
            mainLine = btnLine;
        }
        if (bitRead(tempStatus, 0))
        {
            break;
        }
        sensorLine = map(constrain(analogRead(menuPin), 100, 900), 100, 900, 1, 5);
        if (sensorLine != oldSensorLine)
        {
            oldSensorLine = sensorLine;
            mainLine = sensorLine;
        }

        bgm1.nextBgm();
    } while (1);

    lcd1.noBlink();
    bgm1.stopBgm();

    (*menuPointer[mainLine-1])();
}
