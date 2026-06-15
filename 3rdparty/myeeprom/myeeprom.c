#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include "myeeprom.h"


#define SYS_TIME_ADDR 0x00000400
#define CNF_FILE_CURTIME      "/sdcard/ts/cnf.curtime"

static void dump_time(char *tag)
{

    struct timeval tv;
    double now_s;

    gettimeofday(&tv, NULL);
    now_s = tv.tv_sec + (double)tv.tv_usec * 0.000001;

    printf("\nTIME[%s]: %02fs\n", tag, now_s);

}

static inline __s32 i2c_smbus_access(int file, char read_write, __u8 command, int size, union i2c_smbus_data *data)
{
	struct i2c_smbus_ioctl_data args;

	args.read_write = read_write;
	args.command = command;
	args.size = size;
	args.data = data;
#if 0 
	if(size==2){
		printf("%d ",size);
		printf("%x ",command);
		printf("%04x\n",data->byte);
	}
	if(size==3){
		printf("%x ",command);
		printf("%04x\n",data->word);
	}
#endif
	return ioctl(file,I2C_SMBUS,&args);
}

static inline __s32 i2c_smbus_read_byte(int file)
{
	union i2c_smbus_data data;
	if (i2c_smbus_access(file,I2C_SMBUS_READ,0,I2C_SMBUS_BYTE,&data)) {
		return -1;
	} else {
		return 0x0FF & data.byte;
	}
}

static inline __s32 i2c_smbus_write_byte_data(int file, __u8 command, __u8 value)
{
	union i2c_smbus_data data;
	data.byte = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command, I2C_SMBUS_BYTE_DATA, &data);
}

static inline __s32 i2c_smbus_write_word_data(int file, __u8 command, __u16 value)
{
	union i2c_smbus_data data;
	data.word = value;
	return i2c_smbus_access(file, I2C_SMBUS_WRITE, command, I2C_SMBUS_WORD_DATA, &data);
}

static int i2c_write_2b(struct eeprom *e, __u8 buf[2])
{
	int r;
	r = i2c_smbus_write_byte_data(e->fd, buf[0], buf[1]);
	if(r < 0) {
		fprintf(stderr, "Error i2c_write_2b: %s\n", strerror(errno));
	}
	usleep(10);
	return r;
}

static int i2c_write_3b(struct eeprom *e, __u8 buf[3])
{
	int r;
	r = i2c_smbus_write_word_data(e->fd, buf[0], buf[2] << 8 | buf[1]);
	if(r < 0) {
		fprintf(stderr, "Error i2c_write_3b: %s\n", strerror(errno));
	}
	usleep(10);
	return r;
}


#define CHECK_I2C_FUNC( var, label ) do { if(0 == (var & label)) { fprintf(stderr, "\nError: " #label " function is required. Program halted.\n\n"); exit(1); } } while(0);

int eeprom_open(char *dev_fqn, int addr, int type, struct eeprom* e)
{
	int funcs, fd, r;
	e->fd = e->addr = 0;
	e->dev = 0;
	
	fd = open(dev_fqn, O_RDWR);
	if(fd <= 0) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	if((r = ioctl(fd, I2C_FUNCS, &funcs) < 0)) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}

	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_BYTE_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_READ_WORD_DATA );
	CHECK_I2C_FUNC( funcs, I2C_FUNC_SMBUS_WRITE_WORD_DATA );

	if( ( r = ioctl(fd, I2C_SLAVE, addr)) < 0) {
		fprintf(stderr, "Error eeprom_open: %s\n", strerror(errno));
		return -1;
	}
	e->fd = fd;
	e->addr = addr;
	e->dev = dev_fqn;
	e->type = type;
	return 0;
}

int eeprom_close(struct eeprom *e)
{
	close(e->fd);
	e->fd = -1;
	e->dev = 0;
	e->type = EEPROM_TYPE_UNKNOWN;
	return 0;
}

int eeprom_read_byte(struct eeprom* e, __u16 mem_addr)
{
	int r;
	ioctl(e->fd, BLKFLSBUF);
	if(e->type == EEPROM_TYPE_16BIT) {
		__u8 buf[2] = { (mem_addr >> 8) & 0x0ff, mem_addr & 0x0ff };
		r = i2c_write_2b(e, buf);
	} else {
		fprintf(stderr, "ERR: unknown eeprom type\n");
		return -1;
	}
	if (r < 0) {
		return r;
	}
	r = i2c_smbus_read_byte(e->fd);
	return r;
}

int eeprom_write_byte(struct eeprom *e, __u16 mem_addr, __u8 data)
{
	if(e->type == EEPROM_TYPE_16BIT) {
		__u8 buf[3] = { (mem_addr >> 8) & 0x00ff, mem_addr & 0x00ff, data };
		return i2c_write_3b(e, buf);
	} 
	fprintf(stderr, "ERR: unknown eeprom type\n");
	return -1;
}

#define die_if(a, msg) do { do_die_if( a , msg, __LINE__); } while(0);
void do_die_if(int b, char* msg, int line)
{
	if(!b) {
		return;
	}
	fprintf(stderr, "Error at line %d: %s\n", line, msg);
	fprintf(stderr, "	sysmsg: %s\n", strerror(errno));
	exit(1);
}

int myeeprom_read(int addr, char *buf, int size)
{
	int i,j;

	struct eeprom e;
    char temp[1];

//	die_if(eeprom_open("/dev/i2c/0", 0x50, EEPROM_TYPE_16BIT, &e) < 0, "unable to open eeprom device file (check that the file exists and that it's readable)");

	if (eeprom_open("/dev/i2c/0", 0x50, EEPROM_TYPE_16BIT, &e) < 0) {
		printf("unable to open eeprom device file (check that the file exists and that it's readable\n");
		return -1;
	}

	bzero(buf, size);

	for(i = 0; i < size; i++, addr++, buf++) {
		for (j = 0;j < 3;) {
			usleep(1);
            temp[0]=eeprom_read_byte(&e, addr);
            printf("%02x ",temp[0]);
			if ((*buf = temp[0]) < 0) {
				j++;
				printf("read erro\n");
			//	return -1;
			}

			if ( j >= 3) {
				printf("read erro\n");
				return -1;
			}
			else {
				break;
			}
		}
	//	die_if((*buf = eeprom_read_byte(&e, addr)) < 0, "read error");
	}

	eeprom_close(&e);
	return 1;
}

int myeeprom_write(int addr, char *buf, int size)
{
	int i, j;

	struct eeprom e;

    char temp[1];
//	die_if(eeprom_open("/dev/i2c/0", 0x50, EEPROM_TYPE_16BIT, &e) < 0, "unable to open eeprom device file (check that the file exists and that it's readable)");

	if (eeprom_open("/dev/i2c/0", 0x50, EEPROM_TYPE_16BIT, &e) < 0) {
		printf("unable to open eeprom device file (check that the file exists and that it's readable\n");
		return -1;
	}

	for(i = 0; i < size; i++, addr++, buf++) {
		for (j = 0; j < 3; j++) {
			usleep(1);
			if (eeprom_write_byte(&e, addr, *buf) < 0) {
				printf("write erro\n");
				return -1;
			}
			usleep(1000*5);
            temp[0]=eeprom_read_byte(&e, addr);
            printf("%02x-",temp[0]);
			if (*buf == temp[0]) {
				j = 0;
				printf("write ok\n");
				break;
			}
		}

		if (j >= 3) {
			j = 0;
		}
	//	die_if(eeprom_write_byte(&e, addr, *buf), "write error");
	}

	eeprom_close(&e);
	return 1;
}

/* 写入eep时间 */
int writeSysTimeToE2prom()
{
    time_t raw_time;
    char time_data[4];

    // 获取系统时间
    time(&raw_time);
    time_data[0] = (raw_time>>24)&0xff;
    time_data[1] = (raw_time>>16)&0xff;
    time_data[2] = (raw_time>>8)&0xff;
    time_data[3] = raw_time &0xff;

    return myeeprom_write(SYS_TIME_ADDR, time_data, 4);
}

/* 读取eep时间 */
int getSysTimeFromE2prom(time_t *e2p_time)
{
    char time_data[4];
    int ret = 0;
    ret = myeeprom_read(SYS_TIME_ADDR, time_data, 4);
    *e2p_time = time_data[0]<<24|time_data[1]<<16|time_data[2]<<8|time_data[3];
    return ret;
}

/* 写入emmc时间 */
int writeSysTimeToEmmc()
{
 	time_t raw_time;
    int ret;

    time(&raw_time);

    ret = atomicWriteParam(CNF_FILE_CURTIME,
                              (const char*)&raw_time,
                              sizeof(raw_time));
    return ret;
}

/* 读取emmc时间 */
int getSysTimeFromEmmc(time_t *e2p_time)
{
 	if (e2p_time == NULL) {
        return 0;
    }

    size_t dataLen = 0;
    char* data = atomicReadParam(CNF_FILE_CURTIME, &dataLen);

    if (data != NULL && dataLen == sizeof(time_t)) {
        memcpy(e2p_time, data, sizeof(time_t));
        free(data); 
        return 1;
    }

    if (data != NULL) {
        free(data);
    }
    return 0;
}

/* 原子写入参数 */
int atomicWriteParam(const char* filePath, const char* paramData, size_t dataLen) {
    size_t pathLen = strlen(filePath);
    char* tmpFilePath = (char*)malloc(pathLen + 5); 
    if (!tmpFilePath) {
        return 0;
    }
    strcpy(tmpFilePath, filePath);
    strcat(tmpFilePath, ".tmp");

    int fd = open(tmpFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        free(tmpFilePath);
        return 0;
    }

    ssize_t writeLen = write(fd, paramData, dataLen);
    if (writeLen != (ssize_t)dataLen) {
        close(fd);
        unlink(tmpFilePath);
        free(tmpFilePath);
        return 0;
    }
    if (fsync(fd) < 0) {
        close(fd);
        unlink(tmpFilePath);
        free(tmpFilePath);
        return 0;
    }
    close(fd);

    if (rename(tmpFilePath, filePath) < 0) {
        unlink(tmpFilePath);
        free(tmpFilePath);
        return 0;
    }

    free(tmpFilePath);
    return 1;
}

/* 原子读取参数　*/
char* atomicReadParam(const char* filePath, size_t* outDataLen) {
    if (outDataLen) {
        *outDataLen = 0;
    }

    if (access(filePath, F_OK) < 0) {
        return NULL;
    }

    int fd = open(filePath, O_RDONLY);
    if (fd < 0) {
        return NULL;
    }

    struct stat fileStat;
    if (fstat(fd, &fileStat) < 0) {
        close(fd);
        return NULL;
    }
    off_t fileSize = fileStat.st_size;
    if (fileSize <= 0) {
        close(fd);
        return NULL;
    }

    char* data = (char*)malloc(fileSize);
    if (!data) {
        close(fd);
        return NULL;
    }

    ssize_t readLen = read(fd, data, fileSize);
    close(fd);

    if (readLen != fileSize) {
        free(data);
        return NULL;
    }

    if (outDataLen) {
        *outDataLen = (size_t)fileSize;
    }

    return data;
}

