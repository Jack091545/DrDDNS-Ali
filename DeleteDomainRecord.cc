#include "DeleteDomainRecord.h"
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <iostream>
#include "ConfigHelper.h"

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

DeleteDomainRecord::DeleteDomainRecord(QObject *parent) : Aliyun(parent)
{

}

void DeleteDomainRecord::setRecordId(const QString &recordId)
{
    mRecordId = recordId;
}

void DeleteDomainRecord::onNetworkReadReady()
{
    QByteArray data = mNetworkReply->readAll();
    mNetworkData.append(data);
}

void DeleteDomainRecord::onNetworkFinished()
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

    QCoreApplication::exit();
}

void DeleteDomainRecord::onNetworkError(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)
    qWarning() << "DeleteDomainRecord Error: " + mNetworkReply->errorString();
}

/* 删除DNS解析记录 */
void DeleteDomainRecord::doIt()
{
    QUrlQuery urlQ = publicRequest();
    urlQ.addQueryItem("Action", "DeleteDomainRecord");
    urlQ.addQueryItem("RecordId", mRecordId);
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

    connect(mNetworkReply, &QNetworkReply::readyRead, this, &DeleteDomainRecord::onNetworkReadReady);
    connect(mNetworkReply, &QNetworkReply::finished, this, &DeleteDomainRecord::onNetworkFinished);
    connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), \
        this, &DeleteDomainRecord::onNetworkError);
}


ALI_CLOUD_DNS_NAMESPACE_END
