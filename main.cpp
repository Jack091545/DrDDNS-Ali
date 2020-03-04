#include <QCoreApplication>

#include "Aliyun.h"
#include "DescribeDomainRecords.h"
#include "UpdateDomainRecord.h"
#include "ConfigHelper.h"

using namespace AliyunNameSpace;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    /* 读取配置文件 */
    ConfigHelper confHelper;
    if(!confHelper.load()) {
        QCoreApplication::exit();
        return -1;
    }

#if 0
    DescribeDomainRecords mDescribeDomainRecords;
    mDescribeDomainRecords.setAccessKeyId(confHelper.accessKeyId());
    mDescribeDomainRecords.setAccessKeySecret(confHelper.accessKeySecret());
    mDescribeDomainRecords.setDomain(confHelper.domain());
    mDescribeDomainRecords.doIt();
#endif
    UpdateDomainRecord mUpdateDomainRecord;
    mUpdateDomainRecord.setAccessKeyId(confHelper.accessKeyId());
    mUpdateDomainRecord.setAccessKeySecret(confHelper.accessKeySecret());
    mUpdateDomainRecord.setDomain(confHelper.domain());
    mUpdateDomainRecord.setDomainRecords(confHelper.domainRecords());
    mUpdateDomainRecord.doIt();

    return a.exec();
}

//LTAI4FpMXrNAWu5tnZuhgVAm
//dewee2gfoAq1QvHmmAEidM8eAMnMdK
