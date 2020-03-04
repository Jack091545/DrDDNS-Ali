#include "ConfigHelper.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <iostream>



ConfigHelper::ConfigHelper(QObject *parent) : QObject(parent)
{

}


bool ConfigHelper::load()
{
    QFile file(QCoreApplication::applicationDirPath() + "/DrDDNS-Ali.conf");
    if (!file.exists()) {
        std::cout << QString::fromUtf8("ConfigHelper Error: file not found.").toStdString() << std::endl;
        return false;
    }

    if (!file.open(QIODevice::ReadWrite)) {
        std::cout << QString::fromUtf8("ConfigHelper Error: file can not open.").toStdString() << std::endl;
        return false;
    }
    QByteArray confData = file.readAll();
    file.close();
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



QString ConfigHelper::lastIP()
{
    return mConf["LastIP"].toString();
}

bool ConfigHelper::setLastIP(const QString &ip)
{
    mConf["LastIP"] = ip;
    QFile file(QCoreApplication::applicationDirPath() + "/DrDDNS-Ali.conf");
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
