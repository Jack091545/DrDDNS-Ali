#include "ConfigHelper.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <iostream>



ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{

}

/* 加载配置文件 */
bool ConfigHelper::load()
{
    /* 检查文件夹是否存在，不存在则创建 */
    QDir configDir(QDir::homePath() + "/.config");
    if (!configDir.exists())
        configDir.mkdir(QDir::homePath() + "/.config");

    /* 检查配置文件是否存在，不存在就创建 */
    QByteArray confData;
    QFile file(QDir::homePath() + "/.config/DrDDNS-Ali.conf");
    if (!file.exists()) {
        file.open(QIODevice::WriteOnly);
        file.write("{\"WAN-IP\":\"\"}");
        file.close();
        confData = QByteArray("{\"WAN-IP\":\"\"}");
    } else {
        if (!file.open(QIODevice::ReadOnly)) {
            std::cout << QString::fromUtf8("ConfigHelper Error: file can not open.").toStdString() << std::endl;
            return false;
        }
        confData = file.readAll();
        file.close();
    }

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(confData, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        std::cout << (QString::fromUtf8("ConfigHelper Error: ") + parseError.errorString()).toStdString() << std::endl;
        return false;
    }
    mConf = doc.object();
    return true;
}

QString ConfigHelper::accessKeyId()
{
    return mConf["AccessKeyId"].toString();
}

QString ConfigHelper::accessKeySecret()
{
    return mConf["AccessKeySecret"].toString();

}


QString ConfigHelper::domain()
{
    return mConf["Domain"].toString();
}



QString ConfigHelper::WANIP()
{
    return mConf["WAN-IP"].toString();
}

bool ConfigHelper::setWANIP(const QString &ip)
{
    mConf["WAN-IP"] = ip;
    QFile file(QDir::homePath() + "/.config/DrDDNS-Ali.conf");
    if (!file.exists()) {
        std::cout << QString::fromUtf8("ConfigHelper Error: file not found.").toStdString() << std::endl;
        return false;
    }

    if (!file.open(QIODevice::ReadWrite)) {
        std::cout << QString::fromUtf8("ConfigHelper Error: file can not open.").toStdString() << std::endl;
        return false;
    }
    QJsonDocument doc;
    doc.setObject(mConf);
    file.write(doc.toJson());
    file.flush();
    file.close();
    return true;
}

QList<QMap<QString, QString>> ConfigHelper::domainRecords()
{
    QList<QMap<QString, QString>> list;
    QJsonArray array = mConf["DomainRecords"].toArray();
    for (int i = 0; i < array.size(); i++) {
        QMap<QString, QString> map;
        map.insert("RR", array[i].toObject()["RR"].toString());
        map.insert("RecordId", array[i].toObject()["RecordId"].toString());
        map.insert("Type", array[i].toObject()["Type"].toString());
        list.append(map);
    }
    return list;
}
