#include "UpdateDomainRecord.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrl>
#include <iostream>

ALI_CLOUD_DNS_NAMESPACE_BEGAIN


UpdateDomainRecord::UpdateDomainRecord(QObject *parent) : Aliyun(parent)
{
    connect(&mIfConf, &IfConfig::grabFinished, this, &UpdateDomainRecord::onIpGrabFinished);

}



void UpdateDomainRecord::onIpGrabFinished()
{
    /* 如果是已经同步过了的ip，则不同步 */
    if(!mConfHelper.load()) {
        QCoreApplication::exit(-1);
        return;
    }

    QUrlQuery urlQ = publicRequest();
    urlQ.addQueryItem("Action", "UpdateDomainRecord");
    urlQ.addQueryItem("DomainName", mDomian);

    /* 取出record */
    QMap<QString, QString> map = mRecords.dequeue();
    urlQ.addQueryItem("RR", QUrl::toPercentEncoding(map["RR"]));
    urlQ.addQueryItem("RecordId", map["RecordId"]);
    urlQ.addQueryItem("Type", map["Type"]);

    /* 获取当前的ip */
    urlQ.addQueryItem("Value", (map["Value"] == "Auto") ? mIfConf.ip() : map["Value"]);
    mRecordId = map["RecordId"];
    /* 自动设置WAN IP的情况下，才去对比真实的WAN IP和配置文件中的WAN IP */

    if (map["Value"] == "Auto") {
        if (map["RecordId"] == mIfConf.ip()) {
            QCoreApplication::exit(-1);
            return;
        }
    }

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

    connect(mNetworkReply, &QNetworkReply::readyRead, this, &UpdateDomainRecord::onNetworkReadReady);
    connect(mNetworkReply, &QNetworkReply::finished, this, &UpdateDomainRecord::onNetworkFinished);
    connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), \
        this, &UpdateDomainRecord::onNetworkError);
}

void UpdateDomainRecord::setDomainRecords(const QList<QMap<QString, QString>> records)
{
    for (int i = 0; i < records.size(); i++)
        mRecords.enqueue(records[i]);
}

void UpdateDomainRecord::onNetworkReadReady()
{
    QByteArray data = mNetworkReply->readAll();
    mNetworkData.append(data);
}

void UpdateDomainRecord::onNetworkFinished()
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

    /* 如果没有要设置的记录，把当前的WAN ip写入配置文件 */
    if(!mConfHelper.setIpInfo(mRecordId, mIfConf.ip())) {
        QCoreApplication::exit(-1);
        return;
    }
    QCoreApplication::exit();

}

void UpdateDomainRecord::onNetworkError(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)
    qWarning() << "UpdateDomainRecord Error: " + mNetworkReply->errorString();
}

/* 读取当前的DNS解析记录 */
void UpdateDomainRecord::doIt()
{
    mIfConf.grab();
}

ALI_CLOUD_DNS_NAMESPACE_END
