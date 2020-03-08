#include "AddDomainRecord.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <iostream>

ALI_CLOUD_DNS_NAMESPACE_BEGAIN


AddDomainRecord::AddDomainRecord(QObject *parent) : Aliyun(parent)
{
    connect(&mIfConf, &IfConfig::grabFinished, this, &AddDomainRecord::onIpGrabFinished);

}



void AddDomainRecord::onIpGrabFinished()
{
    QUrlQuery urlQ = publicRequest();
    urlQ.addQueryItem("Action", "AddDomainRecord");
    urlQ.addQueryItem("DomainName", mDomian);

    /* 取出record */
    QMap<QString, QString> map = mRecords.dequeue();
    urlQ.addQueryItem("RR", QUrl::toPercentEncoding(map["RR"]));
    urlQ.addQueryItem("Type", map["Type"]);

    /* 获取当前的ip */
    urlQ.addQueryItem("Value", (map["Value"] == "Auto") ? mIfConf.ip() : map["Value"]);

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

    connect(mNetworkReply, &QNetworkReply::readyRead, this, &AddDomainRecord::onNetworkReadReady);
    connect(mNetworkReply, &QNetworkReply::finished, this, &AddDomainRecord::onNetworkFinished);
    connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), \
        this, &AddDomainRecord::onNetworkError);
}

void AddDomainRecord::setDomainRecords(const QList<QMap<QString, QString>> records)
{
    for (int i = 0; i < records.size(); i++)
        mRecords.enqueue(records[i]);
}

void AddDomainRecord::onNetworkReadReady()
{
    QByteArray data = mNetworkReply->readAll();
    mNetworkData.append(data);
}

void AddDomainRecord::onNetworkFinished()
{
    QJsonParseError jsonParseError;
    QJsonDocument doc = \
            QJsonDocument::fromJson(mNetworkData, &jsonParseError);
    if (jsonParseError.error != QJsonParseError::NoError) {
         QCoreApplication::exit();
         return;
    }

    std::cout << (QString("%1 %2")
                        .arg("RecordId",12)
                        .arg("RequestId", 8)).toStdString() << std::endl;
    std::cout << (QString("%1 %2")
                        .arg(doc.object()["RecordId"].toString(),12)
                        .arg(doc.object()["RequestId"].toString(), 8)).toStdString() << std::endl;
    /* 如果列表里面还有要设置的记录 */
    if(mRecords.count() > 0) {
        doIt();
        return;
    }

    QCoreApplication::exit();
}

void AddDomainRecord::onNetworkError(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)
    qWarning() << "AddDomainRecord Error: " + mNetworkReply->errorString();
}

/* 读取当前的DNS解析记录 */
void AddDomainRecord::doIt()
{
    mIfConf.grab();
}

ALI_CLOUD_DNS_NAMESPACE_END
