#include <QString>
#include <QFile>
#include <QFileInfo>
#include "myfscheck.h"

MyFsCheck myfs;

int MyFsCheck::saveMemoryDetectedFileStaus(QString path)
{
#if 0
    FILE *fp;
    int i;
    int count[2];
    bool bSaveIsOk = false;

    QString filename = path+ "/.memstatus.txt";
    const char* file = QString(filename).toLocal8Bit().constData();

    count[0] = struGsh.nCounter;
    count[1] = 0;

    for(i = 0; i < 3; i++) {
        if (!(fp = fopen(file, "w"))) {
            qDebug("saveMemoryDetectedFileStaus: can not save FileStatus parameters");
            return 0;
        }
        fwrite(&count[0], sizeof(struGsh.nCounter), 1, fp);
        fclose(fp);
        usleep(100000);

        if (!(fp = fopen(file, "r"))) {
            fprintf(stderr, "getMemoryDetectedFileStaus: can not get FileStatus parameters, using default\n");
            return false;
        }
        fread(&count[1], sizeof(int), 1, fp);
        fclose(fp);
        if(count[0] == count[1] ) {
            break;
        }
    }
    return 1;
#endif
}


QString MyFsCheck::getMountDevice(int type, QString path)
{
    QString cmd,dev;
    if(type == 0){
        cmd = "cat /proc/mounts |grep "+ path +"|sed -n '$p' |awk '{print $1}'|sed 's/%//g'";
        dev = g_Runtime().mySystemStr(cmd);
        if(!dev.isEmpty()){
            return dev;
        }
    }
    else{
        cmd = "cat /proc/mounts |grep "+ path +"|sed -n '$p' |awk '{print $2}'|sed 's/%//g'";
        dev = g_Runtime().mySystemStr(cmd);
        if(!dev.isEmpty()){
            return dev;
        }
    }
    return dev;
}

int MyFsCheck::getFileSysType(QString path)
{
    QString cmd = "cat /proc/mounts |grep "+ path +"|sed -n '$p' |awk '{print $3}'|sed 's/%//g'";
    QString type = g_Runtime().mySystemStr(cmd);
    if(!type.isEmpty()){
        if(type == "vfat")
            return FS_FAT;
        else if(type == "ext3")
            return FS_EXT3;
    }
    return CMD_ERR;
}

int MyFsCheck::getMountStaus(QString path)
{
    QString cmd = "cat /proc/mounts |grep "+ path +"|sed -n '$p' |awk '{print $4}'|sed 's/%//g'";
    QString mount = g_Runtime().mySystemStr(cmd);
    if(!mount.isEmpty()){
        QStringList list = mount.split(",");
        if(list.length() > 1){
            if(list.at(0) == "ro")
                return MOUNT_RO;
            else
                return MOUNT_RW;
        }
    }else{
        return MOUNT_FAIL;
    }
    return CMD_ERR;
}


int MyFsCheck::getUsedStaus(QString path)
{
    QString cmd = "df -h |grep "+ path +"|sed -n '$p' |awk '{print $5}'|sed 's/%//g'";
    QString used = g_Runtime().mySystemStr(cmd);
    if(!used.isEmpty()){
        int usedInt = used.toInt();
        if(usedInt > 90)
            return USED_TOO_HIGH;
        else
            return USED_NORMAL;
    }else{
        return DF_FAIL;
    }
    return CMD_ERR;
}
