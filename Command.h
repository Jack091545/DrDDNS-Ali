#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QCoreApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include "Aliyun.h"
#include "DescribeDomainRecords.h"
#include "UpdateDomainRecord.h"
#include "DeleteDomainRecord.h"
#include "AddDomainRecord.h"
#include "ConfigHelper.h"

using namespace AliyunNameSpace;

class Command : public QObject
{
    Q_OBJECT
public:
    explicit Command(QObject *parent = nullptr);
    int exec();
private:
    QCommandLineParser mParser;
    DescribeDomainRecords mDescribeDomainRecords;
    UpdateDomainRecord mUpdateDomainRecord;
    DeleteDomainRecord mDeleteDomainRecord;
    AddDomainRecord mAddDomainRecord;

    QString mAccessKeyId;
    QString mAccessKeySecret;
    QString mDomain;
signals:

public slots:
};

#endif // COMMAND_H
