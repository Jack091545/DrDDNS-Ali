#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <iostream>
#include "Aliyun.h"
#include "DescribeDomainRecords.h"
#include "UpdateDomainRecord.h"
#include "DeleteDomainRecord.h"
#include "ConfigHelper.h"

#define DESCRIPTION "\
This is a Read and Write Aliyun DNS's Tool. Configure file DrDDNS-Ali.conf as following: \n\
{ \n\
    \"LastIP\": \"your WAN ip\", \n\
    \"AccessKeyId\": \"your Aliyun Access Id\", \n\
    \"AccessKeySecret\": \"your Aliyun Access Key Secret\", \n\
    \"Domain\": \"your domain\", \n\
    \"DomainRecords\": [ \n\
        { \n\
            \"RR\":\"www\", \n\
            \"RecordId\":\"3559582367551488\", \n\
            \"Type\":\"A\" \n\
        }, \n\
        { \n\
            \"RR\":\"www\", \n\
            \"RecordId\":\"3559582367551488\", \n\
            \"Type\":\"A\" \n\
        } \n\
    ] \n\
} \n"


using namespace AliyunNameSpace;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("DrDDNS-Ali");  // 应用名称
    QCoreApplication::setApplicationVersion("0.1.0");  // 应用版本号

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", DESCRIPTION));  // 设置应用程序描述信息

    parser.addHelpOption();  // 添加帮助选项 （"-h" 或 "--help"）
    parser.addVersionOption();  // 添加版本选项 ("-v" 或 "--version")

    QCommandLineOption readOption(QStringList() << "r" << "read",
                                   QCoreApplication::translate("main", "Read DNS records of current domain."));
    parser.addOption(readOption);

    QCommandLineOption writeOption(QStringList() << "w" << "write",
                                   QCoreApplication::translate("main", "Write DNS records to current domain."));
    parser.addOption(writeOption);

    QCommandLineOption deleteOption(QStringList() << "d" << "delete",
                                   QCoreApplication::translate("main", "Delete DNS records from current domain's records."), "recordId");
    parser.addOption(deleteOption);

    parser.process(a);

    /* 读取配置文件 */
    ConfigHelper confHelper;
    if (!confHelper.load()) {
        QCoreApplication::exit();
        return -1;
    }
    DescribeDomainRecords mDescribeDomainRecords;
    if (parser.isSet("r")) {

        mDescribeDomainRecords.setAccessKeyId(confHelper.accessKeyId());
        mDescribeDomainRecords.setAccessKeySecret(confHelper.accessKeySecret());
        mDescribeDomainRecords.setDomain(confHelper.domain());
        mDescribeDomainRecords.doIt();

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

    return a.exec();
}

//LTAI4FpMXrNAWu5tnZuhgVAm
//dewee2gfoAq1QvHmmAEidM8eAMnMdK
