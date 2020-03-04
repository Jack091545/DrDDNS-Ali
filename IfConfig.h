#ifndef IFCONFIG_H
#define IFCONFIG_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Aliyun.h"


ALI_CLOUD_DNS_NAMESPACE_BEGAIN

class IfConfig : public QObject
{
    Q_OBJECT
public:
    explicit IfConfig(QObject *parent = nullptr);
    void grab();
    QString ip();
    QString city();
    QString zip();
private:

    QNetworkAccessManager mNetWorkAccessManager;
    QNetworkReply *mNetworkReply;
    QByteArray mNetworkData;

    QString mIp;
    QString mCity;
    QString mZip;

signals:
    void grabFinished();

private slots:
    void onNetworkReadReady();
    void onNetworkFinished();
    void onNetworkError(QNetworkReply::NetworkError errorCode);
};
ALI_CLOUD_DNS_NAMESPACE_END
#endif // IFCONFIG_H
