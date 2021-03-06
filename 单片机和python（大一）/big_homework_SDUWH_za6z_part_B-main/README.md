# 代码说明

本次代码包含四个部分：

> main.py -- 主程序
>
> tools.py  -- 工具函数库
>
> send.py -- 手动发送消息程序
>
> requirements.txt -- 需要的配置

## 代码实现了什么

当某一特定成员（记作成员1）发送消息给应用时，

应用将该消息发送给企业内其他成员；

当企业内其他成员发送消息给应用时，

应用将该消息发送给那一特定成员。

当然，通过tools中写的工具类函数，可以实现更多更丰富的自动收发功能。

## 为了正常运行，请配置以下参数

在main.py中，有一些参数要修改，具体已写在代码上

在tools.py中，需要配置以下参数

`corpId = " "`
`corpsecret = " "`
`token = " "`
`encodingAesKey = " "`
`ouraccess = " "`
`dic = {}`

其中，它们分别为

| 项目           | 说明                                      |
| -------------- | ----------------------------------------- |
| corpId         | 企业id                                    |
| corpsecret     | 应用的sercet信息                          |
| token          | 在配置应用的API接收时所给的凭证           |
| encodingAesKey | 在配置应用的API接收时所给的编码后的AESKey |
| ouraccess      | 我们调用接口时的凭证，可留空              |
| dic            | 一个字典，键为成员Userid，值为姓名        |

此外在tools.py中，你还需要修改agentid，它在第55行

以及第49行需要填写你企业的成员信息

在send.py中，需要配置以下参数

`name_to_id = {}`

| 项目       | 说明                               |
| ---------- | ---------------------------------- |
| name_to_id | 一个字典，键为成员姓名，值为Userid |

## 一些说明

1. 这是一个python初学者写的代码，如有错误或其他问题，在这里表示万分歉意
2. 当应用调用接口的凭证过期后，应用会重新获取凭证并储存，可能会造成延迟
3. 导入了Crypto，适用于Linux，而由于兼容性问题，在Windows上应为Cryptodome

## 感谢您的阅读

山东大学（威海）数科班重案六组在这里感谢您的阅读
