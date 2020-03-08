#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <iostream>
#include "Command.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Command cmd;
    if(cmd.exec() < 0)
        return -1;


    /* 读取配置文件 */
#if 0
    ConfigHelper confHelper;
    if (!confHelper.load()) {
        QCoreApplication::exit();
        return -1;
    }
    DescribeDomainRecords mDescribeDomainRecords;
    if (parser.isSet("r")) {



    }
    UpdateDomainRecord mUpdateDomainRecord;
    if (parser.isSet("w")) {
        mUpdateDomainRecord.setAccessKeyId(confHelper.accessKeyId());
        mUpdateDomainRecord.setAccessKeySecret(confHelper.accessKeySecret());
        mUpdateDomainRecord.setDomain(confHelper.domain());
        mUpdateDomainRecord.setDomainRecords(confHelper.domainRecords());
        mUpdateDomainRecord.doIt();
    }

    DeleteDomainRecord mDeleteDomainRecord;
    if (parser.isSet("d")) {
        mDeleteDomainRecord.setAccessKeyId(confHelper.accessKeyId());
        mDeleteDomainRecord.setAccessKeySecret(confHelper.accessKeySecret());
        mDeleteDomainRecord.setDomain(confHelper.domain());
        mDeleteDomainRecord.setRecordId(parser.value("d"));
        mDeleteDomainRecord.doIt();
    }
#endif
    return a.exec();
}
