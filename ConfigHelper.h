#ifndef CONFIGHELPER_H
#define CONFIGHELPER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
#include <QJsonObject>

class ConfigHelper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigHelper(QObject *parent = nullptr);

    /* 加载配置文件 */
    bool load();

    QString ipInfo(const QString &recordId);
    bool setIpInfo(const QString &recordId, const QString &ip);

private:

    QJsonObject mConf;

signals:

public slots:
};

#endif // CONFIGHELPER_H
