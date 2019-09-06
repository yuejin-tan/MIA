//eepromMgr.cpp
//存储空间管理模块，包括对存储空间的规划，对存储文件的管理、存储、记录等
//copyright @ MIA project
//version 2.0

#include "eepromMgr.h"

char pubBuff[512] = {0};
char nameBuff[10] = {0};

eepromMgr::eepromMgr()
{
    uint8_t tempNum;
    tempNum = EEPROM.read(0);
    uint8_t fucker = 0b01010110;
    if (tempNum != fucker)
    {
        eepromInit();
    }
    EEPROM.get(11, totalSong);
    EEPROM.get(13, spaceUsed);
}
eepromMgr::~eepromMgr() {}

int eepromMgr::findSong(int songNum)
{
    int retAddr = 14;
    byte tempData = 0;
    int tempSong = 0;
    uint8_t fucker = 0b11111111;

    if (songNum > totalSong)
    {
        return -1;
    }
    while (tempSong < songNum)
    {
        do
        {
            retAddr++;
            tempData = EEPROM.read(retAddr);
        } while (tempData != fucker);
        tempSong++;
    }
    retAddr++;
    return retAddr;
}

void eepromMgr::getPassword(void)
{
    char tempData;
    for (int i = 0; i < 10; i++)
    {
        tempData = EEPROM.read(i + 1);
        pubBuff[i] = tempData;
    }
}

void eepromMgr::changePassword(void)
{
    char tempData;
    for (int i = 0; i < 10; i++)
    {
        tempData = nameBuff[i];
        EEPROM.write(i + 1, tempData);
    }
}

int eepromMgr::eepromShow(int info)
{
    int tempNum = -1;
    char tempData = 0;
    int buffPointer = 0;
    uint8_t fucker = 0b00000000;
    if (info == -2)
    {
        return EEPROM.length() -100- spaceUsed;
    }
    else if (info == -1)
    {
        return totalSong;
    }
    else if (info == 0)
    {
        return -1;
    }
    else
    {
        tempNum = findSong(info);
        if (tempNum == -1)
        {
            return -1;
        }
        else
        {
            do
            {
                tempData = EEPROM.read(tempNum);
                nameBuff[buffPointer] = tempData;
                tempNum++;
                buffPointer++;
            } while (tempData != fucker);
            return 0;
        }
    }
}

int eepromMgr::songLoad(int songNum)
{
    int tempAddr = -1;
    uint8_t tempData = 0;
    int buffPointer = 0;
    uint8_t fucker = 0b00000000;
    tempAddr = findSong(songNum);
    if (tempAddr == -1)
    {
        return -1;
    }
    do
    {
        tempData = EEPROM.read(tempAddr);
        nameBuff[buffPointer] = tempData;
        tempAddr++;
        buffPointer++;
    } while (tempData != fucker);
    buffPointer = 0;
    do
    {
        tempData = EEPROM.read(tempAddr);
        pubBuff[buffPointer] = tempData;
        tempAddr++;
        buffPointer++;
    } while (tempData != fucker);
    return 0;
}

int eepromMgr::songSave(void)
{
    int tempAddr = spaceUsed;
    uint8_t tempData = 0;
    int buffPointer = 0;
    uint8_t number = 0b00000000;
    uint8_t number2 = 0b11111111;
    if ((EEPROM.length()-100 - spaceUsed) < (strlen(pubBuff) + strlen(nameBuff) + 3))
    {
        return 1;
    }
    EEPROM.write(tempAddr, number2);
    tempAddr++;
    while (nameBuff[buffPointer] != number)
    {
        EEPROM.write(tempAddr, nameBuff[buffPointer]);
        tempAddr++;
        buffPointer++;
    }
    EEPROM.write(tempAddr, nameBuff[buffPointer]);
    tempAddr++;
    buffPointer = 0;
    while (pubBuff[buffPointer] != number)
    {
        EEPROM.write(tempAddr, pubBuff[buffPointer]);
        tempAddr++;
        buffPointer++;
    }
    EEPROM.write(tempAddr, pubBuff[buffPointer]);
    tempAddr++;
    spaceUsed = tempAddr;
    totalSong++;

    EEPROM.put(11, totalSong);
    EEPROM.put(13, spaceUsed);
    return 0;
}

int eepromMgr::songDelete(int songNum)
{
    int tempAddr1 = 0, tempAddr2 = 0;
    int tempNum=0;
    uint8_t tempData = 15;
    tempAddr1 = findSong(songNum);
    tempAddr2 = findSong(songNum + 1);
    uint8_t fucker = 0b00000000;
    if ((tempAddr1 == -1))
    {
        return -1;
    }
    if ((tempAddr2 == -1))
    {
        EEPROM.write(tempAddr1 - 1, fucker);
        tempNum = spaceUsed;
        spaceUsed = tempAddr1 - 1;
        totalSong--;
        while (tempAddr1 < tempNum)
        {
            EEPROM.write(tempAddr1, fucker);
            tempAddr1++;
        }
        EEPROM.put(11, totalSong);
        EEPROM.put(13, spaceUsed);
        return 0;
    }

    tempNum = spaceUsed;
    spaceUsed -= tempAddr2 - tempAddr1;
    totalSong--;
    while (tempAddr2 < spaceUsed)
    {
        tempData = EEPROM.read(tempAddr2);
        EEPROM.write(tempAddr1, tempData);
        tempAddr1++;
        tempAddr2++;
    }
    while (tempAddr1 <tempNum)
    {
        EEPROM.write(tempAddr1, fucker);
        tempAddr1++;
    }
    EEPROM.put(11, totalSong);
    EEPROM.put(13, spaceUsed);
    return 0;
}

void eepromMgr::eepromInit(void)
{
    int tempint = 28;
    uint8_t initTab[] = {0b01010110,
                         0x00,
                         0x00,
                         0x00,
                         0x00,
                         0x10,
                         0x10,
                         0x10,
                         0x10,
                         0x10,
                         0x10,
                         0x01,
                         0x00};
    uint8_t initTab2[] = {0b11111111,
                          0x61,
                          0x61,
                          0x61,
                          0x00,
                          0X50,
                          0X04,
                          0X0f,
                          0X05,
                          0X0f,
                          0X06,
                          0X0f,
                          0X00};
    for (int i = 0; i < 13; i++)
    {
        EEPROM.write(i, initTab[i]);
    }
    EEPROM.put(13, tempint);
    for (int i = 0; i < 13; i++)
    {
        EEPROM.write(i+15, initTab2[i]);
    }
    for (int i = 28; i < EEPROM.length(); i++)
    {
        EEPROM.write(i, 0);
    }
}