#include "Aliyun.h"
#include <QUrl>
#include <QUrlQuery>
#include <QDateTime>
#include <QDebug>
#include <QtAlgorithms>
#include <QPair>
#include <QCryptographicHash>
#include <QCoreApplication>

/* 阿里云API的服务 */

#define API_VERSION "2015-01-09"


ALI_CLOUD_DNS_NAMESPACE_BEGAIN

Aliyun::Aliyun(QObject *parent) : QObject(parent)
{

}


void Aliyun::setHttpType(const HttpType &type)
{
    mHttpType = type;
}


/* 产生一个随机数字符串 */
QString Aliyun::randString()
{
    int max = 32;
    QString tmp = QString("0123456789abcdefghijklmnopqrstuvwxyz");
    QString str;
    QTime t;
    t = QTime::currentTime();
    qsrand(static_cast<uint>(t.msec() + t.second() * 1000));
    for (int i = 0; i < max; i++)
    {
        int ir = qrand() % tmp.length();
        str[i] = tmp.at(ir);
    }
    return str;
}


/* HMAC-SHA1算法 */
QString Aliyun::hmacSha1(QByteArray key, QByteArray baseString)
{
    int blockSize = 64; // HMAC-SHA-1 block size, defined in SHA-1 standard
    if (key.length() > blockSize) { // if key is longer than block size (64), reduce key length with SHA-1 compression
        key = QCryptographicHash::hash(key, QCryptographicHash::Sha1);
    }

    QByteArray innerPadding(blockSize, char(0x36)); // initialize inner padding with char "6"
    QByteArray outerPadding(blockSize, char(0x5c)); // initialize outer padding with char "quot;
    // ascii characters 0x36 ("6") and 0x5c ("quot;) are selected because they have large
    // Hamming distance (http://en.wikipedia.org/wiki/Hamming_distance)

    for (int i = 0; i < key.length(); i++) {
        innerPadding[i] = innerPadding[i] ^ key.at(i); // XOR operation between every byte in key and innerpadding, of key length
        outerPadding[i] = outerPadding[i] ^ key.at(i); // XOR operation between every byte in key and outerpadding, of key length
    }

    // result = hash ( outerPadding CONCAT hash ( innerPadding CONCAT baseString ) ).toBase64
    QByteArray total = outerPadding;
    QByteArray part = innerPadding;
    part.append(baseString);
    total.append(QCryptographicHash::hash(part, QCryptographicHash::Sha1));
    QByteArray hashed = QCryptographicHash::hash(total, QCryptographicHash::Sha1);
    return hashed.toBase64();
}


/*
    http://alidns.aliyuncs.com/?
    Format=XML
    &AccessKeyId=testid
    &Action=DescribeDomainRecords
    &SignatureMethod=HMAC-SHA1
    &DomainName=example.com
    &SignatureNonce=f59ed6a9-83fc-473b-9cc6-99c95df3856e
    &SignatureVersion=1.0
    &Version=2015-01-09
    &Timestamp=2016-03-24T16:41:54Z

*/

/* 获取公共请求参数 */
QUrlQuery Aliyun::publicRequest()
{
    QUrlQuery urlQ;
    urlQ.addQueryItem("Format", "JSON");
    urlQ.addQueryItem("Version", "2015-01-09");
    urlQ.addQueryItem("AccessKeyId", mAccessKeyId);
    urlQ.addQueryItem("SignatureMethod", "HMAC-SHA1");
    urlQ.addQueryItem("Timestamp", QUrl::toPercentEncoding(QDateTime::currentDateTime().toUTC().toString("yyyy-MM-ddThh:mm:ssZ")));
    urlQ.addQueryItem("SignatureVersion", "1.0");
    urlQ.addQueryItem("SignatureNonce", randString());

    return urlQ;
}

/* 对QUrlQuery进行排序 */
QUrlQuery Aliyun::sortQuery(const QUrlQuery &query)
{
    /* 取出所有的键值对 */
    QList<QPair<QString, QString> > list = query.queryItems();
    QMap<QString, QString> map;
    for (int i = 0; i < list.size(); i++)
        map.insert(list[i].first, list[i].second);
    list.clear();
    QMapIterator<QString, QString> i(map);
    while (i.hasNext()) {
        i.next();
        QPair<QString, QString> pair(i.key(), i.value());
        list.append(pair);
    }
    QUrlQuery tmpQuery;
    tmpQuery.setQueryItems(list);
    return tmpQuery;
}


/* 转换成预签名字符串 */
QString Aliyun::stringPrevSign(const QUrlQuery &query)
{
    return ((mHttpRequestType == Get) ? "GET&%2F&" :  "POST&%2F&") + \
            QUrl::toPercentEncoding(sortQuery(query).toString());
}







ALI_CLOUD_DNS_NAMESPACE_END
