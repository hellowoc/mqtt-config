#ifndef MYFSCHECK_H
#define MYFSCHECK_H

#include "logger.h"
#include "global.h"

#define FS_FAT   1
#define FS_EXT3  2

/* 文件系统及分区状态 */
enum{
    EMMC_BAD = 1,
    CMD_ERR,
    MOUNT_FAIL,
    MOUNT_RW,
    MOUNT_RO,
    DF_FAIL,
    USED_TOO_HIGH,
    USED_NORMAL,
};

typedef struct stu_memory_status_data {
    int nCurRunLevel[3];                        //依次为卡运行区，sd存在，emmc存在  1: sd; 2:emmc
    int nSDMountStatus[2];                      //依次为p1,app状态
    int nSDUsedStatus[2];                       //依次为p1,app状态
    int nEMMCMountStatus[3];                         //依次为p1,app,p2状态
    int nEMMCUsedStatus[3];                         //依次为p1,app,p2状态
}stru_memory_status;

class MyFsCheck : public QObject
{
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER
public:
    explicit MyFsCheck(QObject *parent = 0){};

    int  saveMemoryDetectedFileStaus(QString path);

    QString  getMountDevice(int type, QString path);
    int getFileSysType(QString path);
    int getMountStaus(QString path);
    int getUsedStaus(QString path);

    stru_memory_status struMemoryStatus;
 };

extern MyFsCheck myfs;


#endif // MYFSCHECK_H
