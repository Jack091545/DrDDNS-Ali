#include "IfConfig.h"

#include <QUrl>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

IfConfig::IfConfig(QObject *parent) : QObject(parent)
{

}


void IfConfig::grab()
{
    // http://ifconfig.me/ip
    QUrl url("http://pv.sohu.com/cityjson?ie=utf-8");
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    mNetworkData.clear();
    mNetworkReply = mNetWorkAccessManager.get(request);

    connect(mNetworkReply, &QNetworkReply::readyRead, this, &IfConfig::onNetworkReadReady);
    connect(mNetworkReply, &QNetworkReply::finished, this, &IfConfig::onNetworkFinished);
    connect(mNetworkReply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error), \
        this, &IfConfig::onNetworkError);
}
void IfConfig::onNetworkReadReady()
{
    QByteArray data = mNetworkReply->readAll();
    mNetworkData.append(data);
}

void IfConfig::onNetworkFinished()
{
    mNetworkData = mNetworkData.replace("var returnCitySN = ","").replace(";","");
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(mNetworkData, &parseError);
    if(parseError.error != QJsonParseError::NoError)
            qDebug() << parseError.errorString();

    mIp = doc.object()["cip"].toString();
    mZip = doc.object()["cid"].toString();
    mCity = doc.object()["cname"].toString();

    emit grabFinished();
}

void IfConfig::onNetworkError(QNetworkReply::NetworkError errorCode)
{
    Q_UNUSED(errorCode)
    qWarning() << "DescribeDomainRecords Error: " + mNetworkReply->errorString();
}


QString IfConfig::ip()
{
    return mIp;
}


QString IfConfig::city()
{
    return mCity;
}

QString IfConfig::zip()
{
    return mZip;
}

ALI_CLOUD_DNS_NAMESPACE_END
