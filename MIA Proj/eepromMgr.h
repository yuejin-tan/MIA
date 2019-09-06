//eepromMgr.h
//存储空间管理模块，包括对存储空间的规划，对存储文件的管理、存储、记录等。
//copyright @ MIA project
//version 2.0

#ifndef _EEPROMMGR
#define _EEPROMMGR

#include <EEPROM.h>
#include <arduino.h>

extern char pubBuff[512];
extern char nameBuff[10];

class eepromMgr
{
private:
    int totalSong;
    int spaceUsed;
    //歌曲查找函数,输入歌曲号,返回位置,-1表示无,对内服务
    int findSong(int songNum);

public:
    //初始化函数
    eepromMgr();
    //析构函数
    ~eepromMgr();
    //装载歌曲,将某编号的歌曲装入指定缓存区,返回0代表成功
    int songLoad(int songNum);
    //将缓存区的内容存入eeprom,一般用于传输后存新歌,返回零代表成功,1代表存储空间不足,2代表其他错误
    int songSave(void);
    //删除歌曲,返回0代表成功
    int songDelete(int songNum);
    //读取密码，结果在pub
    void getPassword(void);
    //改密码,新密码默认被在nameBuff中
    void changePassword(void);
    //返回0代表查询成功,参数info：-2代表查询剩余空间,-1代表查询总共有几首歌,正整数n代表查询第n首歌,返回第n首歌歌名（存入nameBuff）
    int eepromShow(int info);
    //eeprom存储器格式化,慎用!
    void eepromInit(void);
};

#endif
