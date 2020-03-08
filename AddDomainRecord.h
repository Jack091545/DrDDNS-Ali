#ifndef ADDDOMAINRECORD_H
#define ADDDOMAINRECORD_H

#include "Aliyun.h"
#include <QQueue>

#include "IfConfig.h"
#include "ConfigHelper.h"

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

class AddDomainRecord : public Aliyun
{
    Q_OBJECT
public:
    explicit AddDomainRecord(QObject *parent = nullptr);
    void setDomainRecords(const QList<QMap<QString, QString>> records);
    void doIt();

private:
    QQueue<QMap<QString, QString>> mRecords;
    IfConfig mIfConf;
    ConfigHelper mConfHelper;
private slots:
    void onNetworkReadReady();
    void onNetworkFinished();
    void onNetworkError(QNetworkReply::NetworkError errorCode);
    void onIpGrabFinished();
};

ALI_CLOUD_DNS_NAMESPACE_END


#endif // UPDATEDOMAINRECORD_H
