#ifndef DESCRIBEDOMAINRECORDS_H
#define DESCRIBEDOMAINRECORDS_H

#include "Aliyun.h"

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

class DescribeDomainRecords : public Aliyun
{
public:
    explicit DescribeDomainRecords(QObject *parent = nullptr);
    void doIt();


private slots:
    void onNetworkReadReady();
    void onNetworkFinished();
    void onNetworkError(QNetworkReply::NetworkError errorCode);

};

ALI_CLOUD_DNS_NAMESPACE_END

#endif // DESCRIBEDOMAINRECORDS_H
