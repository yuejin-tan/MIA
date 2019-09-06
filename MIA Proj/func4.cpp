//func4.cpp
//功能4
//copyright @ MIA project
//version 2.0

#include <avr/pgmspace.h>
#include "led.h"
#include "button.h"
#include <LiquidCrystal.h>
#include "eepromMgr.h"
#include "miaTool.h"
#include "sensor.h"
#include "showu8g2.h"

extern eepromMgr eepromMgr1;
extern led led1;
extern button button1;
extern LiquidCrystal lcd1;

void explorerDelete(int songNum)
{
    int buttonState;
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("storage manager"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("delete?"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("ok or back"));
explorerDeleteloop1:
    buttonState = button1.buttonCheck();
    switch (buttonState)
    {
    case 1:
        eepromMgr1.songDelete(songNum);
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print(F("delete succeed!"));
        delay(1000);
        break;
    default:
        goto explorerDeleteloop1;
        break;
    case 8:
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print(F("getting back"));
        delay(500);
        break;
    }
}

void exploreFile()
{
    int buttonState = 0;
    int line = 1, oldline = 0;
    lcd1.setCursor(0, 0);
    lcd1.print(F("embeded exploer     "));
    while (1)
    {
        if (line != oldline)
        {
            exploreShow(line);
        }
        oldline = line;
        buttonState = button1.buttonCheck();
        switch (buttonState)
        {
        case 1:
            explorerDelete(line);
            lcd1.setCursor(0, 0);
            lcd1.print(F("embeded exploer     "));
            exploreShow(line);
            break;
        case 2:
            line++;
            break;
        case 4:
            if (line > 1)
            {
                line--;
            }
            break;
        case 8:
            goto explorerout;
        }
    }
explorerout:
    return;
}

void download()
{
    int buffPointer = 0;
    uint8_t buttonState = 0;

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("Connecting To the PC"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("trying to establish"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("Bluetooth Connection"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("press back to exit.."));

    Serial1.begin(9600);
    while (bluetooth1.BTLinkStatus() == 0)
    {
        led1.ledcycle(7);
        buttonState = button1.buttonCheck();
        if (buttonState == 8)
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("not connected, exit"));
            Serial1.flush();
            Serial1.end();
            goto stopDownload;
        }
    }
    Serial1.println(F("link ok"));
downloadloop1:
    //传输
    while (Serial1.available() < 1)
    {
        led1.ledcycle(7);
        buttonState = button1.buttonCheck();
        if (buttonState == 8)
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("not finish,giving up"));
            Serial1.flush();
            Serial1.end();
            goto stopDownload;
        }
    }
    if (Serial1.peek() != 0)
    {
        pubBuff[buffPointer++] = Serial1.read();
        Serial1.println(pubBuff[buffPointer - 1]);
        goto downloadloop1;
    }

    pubBuff[buffPointer++] = Serial1.read();
    buffPointer = 0;
    Serial1.println(F("name?"));

downloadloop2:
    while (Serial1.available() < 1)
    {
        led1.ledcycle(7);
        buttonState = button1.buttonCheck();
        if (buttonState == 8)
        {
            lcd1.clear();
            lcd1.setCursor(0, 0);
            lcd1.print(F("not finish,giving up"));
            Serial1.flush();
            Serial1.end();
            goto stopDownload;
        }
    }
    if (Serial1.peek() != 0)
    {
        nameBuff[buffPointer++] = Serial1.read();
        Serial1.println(nameBuff[buffPointer - 1]);
        goto downloadloop2;
    }
    nameBuff[buffPointer++] = Serial1.read();
    while (buffPointer < 10)
    {
        nameBuff[buffPointer++] = 0;
    }

    Serial1.println(F("download ok"));

    int debug;
    if (debug = eepromMgr1.songSave())
    {
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print(F("save error "));
        lcd1.print(debug);
        goto stopDownload;
    }

    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("download succeed"));
    //显示祝贺图片
    showCongratulation();

stopDownload:
    Serial1.flush();
    Serial1.end();
    delay(2000);
}

void func4(void)
{
    int buttonState = 0;
    lcd1.clear();
    lcd1.setCursor(0, 0);
    lcd1.print(F("file system"));
    lcd1.setCursor(0, 1);
    lcd1.print(F("f1:explore file"));
    lcd1.setCursor(0, 2);
    lcd1.print(F("f2:download"));
    lcd1.setCursor(0, 3);
    lcd1.print(F("press back to exit"));

func4loop1:
    buttonState = button1.buttonCheck();
    switch (buttonState)
    {
    case 2:
        exploreFile();
        break;
    case 4:
        download();
        break;
    case 8:
        break;
    default:
        goto func4loop1;
        break;
    }
}