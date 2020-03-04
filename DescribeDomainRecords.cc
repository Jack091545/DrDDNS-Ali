#include "DescribeDomainRecords.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include "ConfigHelper.h"

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

DescribeDomainRecords::DescribeDomainRecords(QObject *parent) : Aliyun(parent)
{

}


void DescribeDomainRecords::onNetworkReadReady()
{
    QByteArray data = mNetworkReply->readAll();
    mNetworkData.append(data);
}

void DescribeDomainRecords::onNetworkFinished()
{
    QJsonParseError jsonParseError;
    QJsonDocument doc = \
            QJsonDocument::fromJson(mNetworkData, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
         QCoreApplication::exit();
         return;
    }
    QJsonArray array = doc.object()["DomainRecords"].
            toObject()["Record"].toArray();
    std::cout << (QString("%1 %2 %3 %4 %5 %6 %7 %8 %9")
                    .arg("RR",12)
                    .arg("Line", 8)
                    .arg("Status", 9)
                    .arg("Locked", 6)
                    .arg("Type", 6)
                    .arg("RecordId", 20)
                    .arg("TTL", 5)
                    .arg("DomainName", 20)
                    .arg("Value")).toStdString() << std::endl;
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        std::cout << (QString("%1 %2 %3 %4 %5 %6 %7 %8 %9")
                        .arg(obj["RR"].toString(),12)
                        .arg(obj["Line"].toString(), 8)
                        .arg(obj["Status"].toString(), 9)
                        .arg(obj["Locked"].toBool()? "true" : "false", 6)
                        .arg(obj["Type"].toString(), 6)
                        .arg(obj["RecordId"].toString(), 20)
                        .arg(obj["TTL"].toInt(), 5)
                        .arg(obj["DomainName"].toString(), 20)
                        .arg(obj["Value"].toString())).toStdString() << std::endl;
    }

    QCoreApplication::exit();
}

void DescribeDomainRecords::onNetworkError(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)
    qWarning() << "DescribeDomainRecords Error: " + mNetworkReply->errorString();
}

/* 读取当前的DNS解析记录 */
void DescribeDomainRecords::doIt()
{
    QUrlQuery urlQ = publicRequest();
    urlQ.addQueryItem("Action", "DescribeDomainRecords");
    urlQ.addQueryItem("DomainName", mDomian);

    /* 获取Signature */
    QString prevString = stringPrevSign(urlQ);
    QString hash = hmacSha1((mAccessKeySecret + "&").toUtf8() , prevString.toUtf8());
    /* 组成完整的url */
    urlQ.addQueryItem("Signature", QUrl::toPercentEncoding(hash));

    QUrl url;
    url.setHost(API_ADDR);
    url.setQuery(sortQuery(urlQ));

    QNetworkRequest request;
    request.setUrl("http:" + url.toString());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    mNetworkData.clear();
    mNetworkReply = mNetWorkAccessManager.get(request);

    connect(mNetworkReply, &QNetworkReply::readyRead, this, &DescribeDomainRecords::onNetworkReadReady);
    connect(mNetworkReply, &QNetworkReply::finished, this, &DescribeDomainRecords::onNetworkFinished);
    connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), \
        this, &DescribeDomainRecords::onNetworkError);
}


ALI_CLOUD_DNS_NAMESPACE_END
