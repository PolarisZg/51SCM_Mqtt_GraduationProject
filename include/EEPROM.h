#ifndef		__EEPROM_H
#define		__EEPROM_H

#include	"config.h"









//sfr IAP_DATA    =   0xC2;           //IAP数据寄存器
//sfr IAP_ADDRH   =   0xC3;           //IAP地址寄存器高字节
//sfr IAP_ADDRL   =   0xC4;           //IAP地址寄存器低字节
//sfr IAP_CMD     =   0xC5;           //IAP命令寄存器
//sfr IAP_TRIG    =   0xC6;           //IAP命令触发寄存器
//sfr IAP_CONTR   =   0xC7;           //IAP控制寄存器

#define CMD_IDLE    0               //空闲模式
#define CMD_READ    1               //IAP字节读命令
#define CMD_PROGRAM 2               //IAP字节编程命令
#define CMD_ERASE   3               //IAP扇区擦除命令

#define ENABLE_IAP 0x80           //if SYSCLK<30MHz
//#define ENABLE_IAP 0x81           //if SYSCLK<24MHz
//#define ENABLE_IAP  0x82            //if SYSCLK<20MHz
//#define ENABLE_IAP 0x83           //if SYSCLK<12MHz

//测试地址
#define IAP_ADDRESS 0xe400//iap系列为58K，0xE800，最后1K作为EEPROM使用





unsigned char IapReadByte(unsigned int addr);
void IapProgramByte(unsigned int addr, unsigned char dat);
void IapEraseSector(unsigned int addr);


#endif
