# DrDDNS-Ali

DrDDNS-Ali是一个可以增删改查阿里云域名解析记录的工具。使用阿里云提供的管理 [接口](https://help.aliyun.com/document_detail/29739.html?spm=a2c4g.11186631.6.616.524329fcYUjGpb "阿里云接口")。使用Qt5编写，所以可以在Windows/Linux/Mac OS X上使用，只需要编译一下即可。这个工具诞生的目的，是实现动态地自动地设置域名的解析记录，方便搭建自己的云端。在使用这个工具的时候，需要以来操作系统提供的自动运行脚本的机制。比如Linux中的`crontab`,windows的`计划任务`。另外，仍然需要网络供应商提供公网IP到你所要搭建的环境的调制解调器上。在这个调制解调器上添加对内网服务器的端口映射。

## 编译
如果有QtCreator的话，使用QtCreator打开DrDDNS-Ali.pro，设置好编译套件后选择release编译即可。或者
```
qmake DrDDNS-Ali.pro 
make                 
```

## 使用
| 参数 | 格式 | 解释 |
| ------ | ------ | ------ |
| --print | 无参数 | 打印当前域名解析记录，依赖--id，--secret，--domain |
| --remove | 无参数 | 删除当前域名解析记录，依赖--id，--secret，--domain，--record `"*;*;RecordId;*"`,非`"*"`需要填写 |
| --append | 无参数 | 添加域名解析记录，依赖--id，--secret，--domain，--record `"RR;Type;*;Value"`,非`"*"`需要填写 |
| --update | 无参数 | 添加域名解析记录，依赖--id，--secret，--domain，--record `"RR;Type;RecordId;Value"`,非`"*"`需要填写 |
| --id | 带参数 | 参数为[阿里云RAM鉴权用户](https://ram.console.aliyun.com/roles)的AccessKeyId |
| --secret | 带参数 | 参数为[阿里云RAM鉴权用户](https://ram.console.aliyun.com/roles)accessKeySecret |
| --domain | 带参数 | 参数为要调整的域名 |
| --record | <"RR;Type;RecordId;Value"> | RR填写主机记录，Type填写记录类型，RecordId填写记录ID，Value填写记录值 |




