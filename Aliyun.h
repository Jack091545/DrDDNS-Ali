#ifndef ALICLOUDDNS_H
#define ALICLOUDDNS_H

#include <QObject>
#include <QMetaType>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#define API_ADDR "alidns.aliyuncs.com"

#define ALI_CLOUD_DNS_NAMESPACE_BEGAIN namespace AliyunNameSpace {
#define ALI_CLOUD_DNS_NAMESPACE_END }

ALI_CLOUD_DNS_NAMESPACE_BEGAIN

/* http类型，默认使用http */
enum HttpType {
    Http = 0,
    Https = 1
};


enum HttpRequestType {
    Get = 0,
    Post = 1
};


class Aliyun : public QObject
{
    Q_OBJECT
public:
    explicit Aliyun(QObject *parent = nullptr);

    /* 设置http类型，默认使用http */
    inline void setHttpType(const HttpType &type);

    /* 设置阿里云颁发给用户的访问服务所用的密钥ID */
    inline void setAccessKeyId(const QString &keyId) { mAccessKeyId = keyId; }
    inline void setAccessKeySecret(const QString &keySecret) { mAccessKeySecret = keySecret; }

    /* 设置域名 */
    inline void setDomain(const QString &domain) { mDomian = domain; }

protected:

    HttpType mHttpType = Http;
    HttpRequestType mHttpRequestType = Get;


    QString mAccessKeyId;
    QString mAccessKeySecret;
    QString mDomian;

    QNetworkAccessManager mNetWorkAccessManager;
    QNetworkReply *mNetworkReply;
    QByteArray mNetworkData;

    /* 获取公共结构参数 */
    QUrlQuery publicRequest();
    /* 对QUrlQuery进行排序 */
    QUrlQuery sortQuery(const QUrlQuery &query);

    /* 获取HMAC-SHA1加密前的字符串 */
    QString stringPrevSign(const QUrlQuery &query);
    /* 获取HMAC-SHA1加密 */
    QString hmacSha1(QByteArray key, QByteArray baseString);
    /* 获取随机字符串 */
    QString randString();

    virtual void doIt() = 0;

signals:


public slots:


};

ALI_CLOUD_DNS_NAMESPACE_END

#endif // ALICLOUDDNS_H
