#ifndef DELETEDOMAINRECORD_H
#define DELETEDOMAINRECORD_H

#include "Aliyun.h"

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

class DeleteDomainRecord : public Aliyun
{
public:
    explicit DeleteDomainRecord(QObject *parent = nullptr);
    void setRecordId(const QString &recordId);
    void doIt();

private:
    QString mRecordId;
private slots:
    void onNetworkReadReady();
    void onNetworkFinished();
    void onNetworkError(QNetworkReply::NetworkError errorCode);

};

ALI_CLOUD_DNS_NAMESPACE_END

#endif // DESCRIBEDOMAINRECORDS_H
