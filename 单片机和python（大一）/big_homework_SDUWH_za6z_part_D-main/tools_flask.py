import hashlib
from base64 import b64decode, b64encode
from Crypto.Cipher import AES
import random
import socket
import struct
import xml.etree.cElementTree as ET
import requests
import re
corpId = "------------------------"
token = "------------"
encodingAesKey = "------------"
ouraccess = "---------------------------------"
dic = {"BaiJinFan": "白锦帆",
    "GongChenFeng": "宫晨峰",
    "lintong": "李晓峰",
    "8bff448a58c8315307d6d1761b0ed267": "孙阳",
    "rearrange": "王婧怡学姐"}
did = ["ok","我生病了","失火了","电路故障","其他求助"]
ju = False
todo = ""


def urlvary(msgsignatrue, timestamp, nonce, echostr):
    thelist = sorted([token, timestamp, nonce, echostr])
    need = ""
    for i in range(4):
        need += thelist[i]
    result = hashlib.sha1(need.encode("utf-8")).hexdigest()
    if result == msgsignatrue:
        return True
    else:
        return False


def decode_echo(sth):
    need_aes = b64decode(sth)
    aeskey = b64decode(encodingAesKey + "=")
    aes = AES.new(aeskey, AES.MODE_CBC, aeskey[:16])
    plain_text = aes.decrypt(need_aes)
    content = plain_text[16:]
    xml_len = socket.ntohl(struct.unpack("I", content[: 4])[0])
    xml_content = content[4:xml_len+4]
    from_receiveid = content[xml_len+4:]
    return xml_content


def send_to(who, text):
    global ouraccess
    url = "https://qyapi.weixin.qq.com/cgi-bin/message/send?access_token=" + ouraccess
    if who == "all":
        target = "BaiJinFan|GongChenFeng|lintong|8bff448a58c8315307d6d1761b0ed267"
    else:
        target = who
    data ={
        "touser": target,
        "msgtype": "text",
        "agentid": 1000002,
        "text": {
                 "content": text
                },
        "safe": 0,
        "enable_id_trans": 0,
        "enable_duplicate_check": 0
        }
    req = requests.post(url, json=data)
    errcode = re.findall(r"errcode\":(.+?),\"errmsg", req.text)[0]
    if errcode == "40014" or errcode == "42001":
        ouraccess = get_access_token()
        send_to(who, text)
    else:
        return errcode


def tiqu(str):
    xml_tiqu = ET.fromstring(str)
    encrypt = xml_tiqu.find("Encrypt")
    return encrypt.text


def get_access_token():
    url = 'https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=------------------------------'
    req = requests.get(url)
    new = re.findall(r"access_token\":\"(.+?)\",\"expires_in", req.text)
    return new[0]


def find(sth):
    text = [0]
    msg_type = re.findall(r"<MsgType><!\[CDATA\[(.+?)\]\]></MsgType>", sth)
    sender = re.findall(r"<FromUserName><!\[CDATA\[(.+?)\]\]></FromUserName>", sth)
    if msg_type[0] != "text":
        text[0] = 0
    else:
        text = re.findall(r"<Content><!\[CDATA\[(.+?)\]\]></Content>", sth)
    return sender[0], text[0]

