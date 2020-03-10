#include "Command.h"
#include <QDebug>
#include <iostream>

using namespace std;

#define DESCRIPTION "This is a Aliyun domain's Record Tool, " \
                    "It can be use to grab your WAN-IP and " \
                    "changed your domain's record with WAN-IP automatically. "

Command::Command(QObject *parent) : QObject(parent)
{
    QCoreApplication::setApplicationName("DrDDNS-Ali");  // 应用名称
    QCoreApplication::setApplicationVersion("0.1.2");  // 应用版本号
    QCoreApplication::setOrganizationName("Matsumoto.Gioomei");
    QCoreApplication::setOrganizationDomain("<Matsumoto.Gioomei@protonmail.ch>");

    mParser.setApplicationDescription(QCoreApplication::translate("main", DESCRIPTION));  // 设置应用程序描述信息

    mParser.addHelpOption();
    mParser.addVersionOption();

    QCommandLineOption printOp(QStringList() << "print",
                                   QCoreApplication::translate("main", "Print domain's records."));
    mParser.addOption(printOp);

    QCommandLineOption updateOp(QStringList() << "update",
                                   QCoreApplication::translate("main", "Update domain's record, --record format is <\"RR;Type;RecordId;Value\">"));
    mParser.addOption(updateOp);

    QCommandLineOption deleteOp(QStringList() << "remove",
                                   QCoreApplication::translate("main", "Remove domain's record, --record format is <\"*;*;RecordId;*\">"));
    mParser.addOption(deleteOp);

    QCommandLineOption appendOp(QStringList() << "append",
                                   QCoreApplication::translate("main", "Append domain's record, --record format is <\"RR;Type;*;Value\">"));
    mParser.addOption(appendOp);

    /*
    QCommandLineOption configFileOption(QStringList() << "config",
                                   QCoreApplication::translate("main", "Specified a confiure file for use."), "file");
    mParser.addOption(configFileOption);
    */

    QCommandLineOption accessKeyIdOption(QStringList() << "id",
                                   QCoreApplication::translate("main", "Specified AccessKeyId from aliyun for account verify."), "accessKeyId");
    mParser.addOption(accessKeyIdOption);

    QCommandLineOption accessKeySecretOption(QStringList() << "secret",
                                   QCoreApplication::translate("main", "Specified AccessKeySecret from aliyun for account verify."), "accessKeySecret");
    mParser.addOption(accessKeySecretOption);

    QCommandLineOption domainOption(QStringList() << "domain",
                                   QCoreApplication::translate("main", "Domain you want to operate."), "domian");
    mParser.addOption(domainOption);

    QCommandLineOption domainRecordOption(QStringList() << "record",
                                   QCoreApplication::translate("main", "Set the records you want to update,format is <\"RR;Type;"
                                                                       "RecordId;Value\">.If you want to set to obtain the IP automatically, "
                                                                       "enter \"Auto\" in the Value field."),
                                          "\"RR;Type;RecordId;Value\"");
    mParser.addOption(domainRecordOption);

    mParser.process(*QCoreApplication::instance());
}

/* 执行命令 */
int Command::exec()
{
    mAccessKeyId.clear();
    mAccessKeySecret.clear();
    mDomain.clear();

    /* 检查accessKeyId */
    if (!mParser.isSet("id")) {
        std::cout << "The required parameter <AccessKeyId> is not set." << std::endl;
        return -1;
    }
    mAccessKeyId = mParser.value("id");

    /* 检查accessKeySecret */
    if (!mParser.isSet("secret")) {
        std::cout << "The required parameter <AccessKeySecret> is not set." << std::endl;
        return -1;
    }
    mAccessKeySecret = mParser.value("secret");

    /* 检查domian */
    if (!mParser.isSet("domain")) {
        std::cout << "The required parameter <Domian> is not set." << std::endl;
        return -1;
    }
    mDomain = mParser.value("domain");

    /* 显示当前的域名解析记录 */
    if (mParser.isSet("print")) {
        mDescribeDomainRecords.setAccessKeyId(mAccessKeyId);
        mDescribeDomainRecords.setAccessKeySecret(mAccessKeySecret);
        mDescribeDomainRecords.setDomain(mDomain);
        mDescribeDomainRecords.doIt();
        return 0;
    }

    /* 更新当前的域名解析记录 */
    if (mParser.isSet("update")) {

        mUpdateDomainRecord.setAccessKeyId(mAccessKeyId);
        mUpdateDomainRecord.setAccessKeySecret(mAccessKeySecret);
        mUpdateDomainRecord.setDomain(mDomain);

        QString updateValue = mParser.value("record");
        QStringList updateValues = updateValue.split(";");
        if(updateValues.size() != 4) {
            std::cout << "The required parameter <RR;Type;RecordId;Value> is not set." << std::endl;
            return -1;
        }
        QMap<QString, QString> map;
        map.insert("RR", updateValues[0]);
        map.insert("Type", updateValues[1]);
        map.insert("RecordId", updateValues[2]);
        map.insert("Value", updateValues[3]);
        QList<QMap<QString, QString>> list;
        list.append(map);
        mUpdateDomainRecord.setDomainRecords(list);
        mUpdateDomainRecord.doIt();
        return 0;
    }
    /* 删除当前域名解析记录中的一条 */
    if (mParser.isSet("remove")) {
        mDeleteDomainRecord.setAccessKeyId(mAccessKeyId);
        mDeleteDomainRecord.setAccessKeySecret(mAccessKeySecret);
        mDeleteDomainRecord.setDomain(mDomain);
        QString updateValue = mParser.value("record");
        QStringList updateValues = updateValue.split(";");
        if(updateValues.size() != 4) {
            std::cout << "The required parameter <RR;Type;RecordId;Value> is not set." << std::endl;
            return -1;
        }
        mDeleteDomainRecord.setRecordId(updateValues[2]);
        mDeleteDomainRecord.doIt();
        return 0;
    }

    /* 更新当前的域名解析记录 */
    if (mParser.isSet("append")) {

        mAddDomainRecord.setAccessKeyId(mAccessKeyId);
        mAddDomainRecord.setAccessKeySecret(mAccessKeySecret);
        mAddDomainRecord.setDomain(mDomain);

        QString updateValue = mParser.value("record");
        QStringList updateValues = updateValue.split(";");
        if(updateValues.size() != 4) {
            std::cout << "The required parameter <RR;Type;RecordId;Value> is not set." << std::endl;
            return -1;
        }
        QMap<QString, QString> map;
        map.insert("RR", updateValues[0]);
        map.insert("Type", updateValues[1]);
        map.insert("Value", updateValues[3]);
        QList<QMap<QString, QString>> list;
        list.append(map);
        mAddDomainRecord.setDomainRecords(list);
        mAddDomainRecord.doIt();
        return 0;
    }
    return -1;
}
