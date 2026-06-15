/* x86_64 stub for myeeprom - no I2C hardware, use file-based simulation */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include "myeeprom.h"

static char eeprom_file[256] = "/tmp/eeprom_stub.bin";
static int eeprom_inited = 0;

static void ensure_eeprom_file(void)
{
    if (eeprom_inited) return;
    struct stat st;
    if (stat(eeprom_file, &st) != 0) {
        /* Create empty eeprom file, 4096 bytes */
        FILE *f = fopen(eeprom_file, "wb");
        if (f) {
            char zero[4096] = {0};
            fwrite(zero, 1, sizeof(zero), f);
            fclose(f);
        }
    }
    eeprom_inited = 1;
}

int myeeprom_write(int addr, char *buf, int size)
{
    ensure_eeprom_file();
    FILE *f = fopen(eeprom_file, "r+b");
    if (!f) return 0;
    fseek(f, addr, SEEK_SET);
    size_t written = fwrite(buf, 1, size, f);
    fclose(f);
    return (written == (size_t)size) ? 1 : 0;
}

int myeeprom_read(int addr, char *buf, int size)
{
    ensure_eeprom_file();
    FILE *f = fopen(eeprom_file, "rb");
    if (!f) return 0;
    fseek(f, addr, SEEK_SET);
    size_t nread = fread(buf, 1, size, f);
    fclose(f);
    return (nread == (size_t)size) ? 1 : 0;
}

int writeSysTimeToE2prom()
{
    ensure_eeprom_file();
    time_t now = time(NULL);
    return myeeprom_write(0x100, (char*)&now, sizeof(now));
}

int getSysTimeFromE2prom(time_t *e2p_time)
{
    return myeeprom_read(0x100, (char*)e2p_time, sizeof(*e2p_time));
}

int writeSysTimeToEmmc()
{
    return writeSysTimeToE2prom(); /* same stub */
}

int getSysTimeFromEmmc(time_t *e2p_time)
{
    return getSysTimeFromE2prom(e2p_time);
}

char* atomicReadParam(const char* filePath, size_t* outDataLen)
{
    FILE *f = fopen(filePath, "rb");
    if (!f) {
        if (outDataLen) *outDataLen = 0;
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = (char*)malloc(len + 1);
    if (buf) {
        fread(buf, 1, len, f);
        buf[len] = '\0';
    }
    fclose(f);
    if (outDataLen) *outDataLen = len;
    return buf;
}

int atomicWriteParam(const char* filePath, const char* paramData, size_t dataLen)
{
    char tmp[512];
    snprintf(tmp, sizeof(tmp), "%s.tmp", filePath);
    FILE *f = fopen(tmp, "wb");
    if (!f) return 0;
    size_t written = fwrite(paramData, 1, dataLen, f);
    fclose(f);
    if (written != dataLen) {
        unlink(tmp);
        return 0;
    }
    rename(tmp, filePath);
    return 1;
}
