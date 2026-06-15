#ifndef _MYEEPROM_H_
#define _MYEEPROM_H_


#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/time.h>


#define EEPROM_TYPE_UNKNOWN 0
#define EEPROM_TYPE_16BIT 1

#ifdef __cplusplus
extern "C"{
#endif
struct eeprom
{
	char *dev;	// device
	int addr;	// address
	int fd;		// file descriptor
	int type; 	// type
};

// API
int myeeprom_write(int addr, char *buf, int size);
int myeeprom_read(int addr, char *buf, int size);
int writeSysTimeToE2prom();
int getSysTimeFromE2prom(time_t *e2p_time);
int writeSysTimeToEmmc();
int getSysTimeFromEmmc(time_t *e2p_time);

/**
 * @brief 读取文件全部内容
 * @param filePath 文件路径
 * @param outDataLen 输出：读取到的数据长度
 * @return 成功返回数据缓冲区（需调用 free() 释放），失败返回 NULL
 */
char* atomicReadParam(const char* filePath, size_t* outDataLen); 

/**
 * @brief 原子写入文件（安全写入，防止文件损坏）
 * @param filePath 目标文件路径
 * @param paramData 要写入的数据缓冲区
 * @param dataLen 数据长度
 * @return 成功返回 true(1)，失败返回 false(0)
 */
int atomicWriteParam(const char* filePath, const char* paramData, size_t dataLen);

#ifdef __cplusplus
}
#endif
#endif /* _MYEEPROM_H_ */

