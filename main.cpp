#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <iostream>
#include "Aliyun.h"
#include "DescribeDomainRecords.h"
#include "UpdateDomainRecord.h"
#include "ConfigHelper.h"

using namespace AliyunNameSpace;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCoreApplication::setApplicationName("DrDDNS-Ali");  // 应用名称
    QCoreApplication::setApplicationVersion("0.1.0");  // 应用版本号

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("main", "Qt"));  // 设置应用程序描述信息

    parser.addHelpOption();  // 添加帮助选项 （"-h" 或 "--help"）
    parser.addVersionOption();  // 添加版本选项 ("-v" 或 "--version")

    QCommandLineOption readOption(QStringList() << "r" << "read",
                                   QCoreApplication::translate("main", "Read DNS records of current domain."));
    parser.addOption(readOption);

    QCommandLineOption writeOption(QStringList() << "w" << "write",
                                   QCoreApplication::translate("main", "Write DNS records to current domain."));
    parser.addOption(writeOption);

    parser.process(a);


    if (parser.isSet("r") && parser.isSet("w")) {
        std::cout << QString("Error : Read is not allowed when Wite, please read help by -h.").toStdString() << std::endl;
        return -1;
    }

    if (!parser.isSet("r") && !parser.isSet("w")) {
        std::cout << QString("Error : Read or Wite is not set, please read help by -h.").toStdString() << std::endl;
        return -1;
    }

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

    return a.exec();
}

//LTAI4FpMXrNAWu5tnZuhgVAm
//dewee2gfoAq1QvHmmAEidM8eAMnMdK
